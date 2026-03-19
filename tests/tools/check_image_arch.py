#!/usr/bin/env python3
"""Check Docker images in environments.toml for multi-arch support.

Finds all environments whose Docker images are missing amd64 or arm64 variants.
Designed to run as a CI checker in GitHub Actions.

Uses the Docker Hub REST API. Note that PAT/OAT tokens must use the "JWT"
authorization scheme, not "Bearer" (Docker Hub rejects Bearer for these tokens).

Usage:
    python3 check_image_arch.py
    python3 check_image_arch.py --token dckr_pat_xxx
    python3 check_image_arch.py --token dckr_oat_xxx --format json
"""

import argparse
import json
import sys
import tomllib
import time
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path

import requests

DOCKERHUB_API = "https://hub.docker.com/v2/repositories"
DEFAULT_ARCHS = "amd64,arm64"
MAX_RETRIES = 3
RETRY_BACKOFF = 5


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Check Docker images in environments.toml for multi-arch support.",
    )
    parser.add_argument(
        "-f",
        "--file",
        type=Path,
        default=Path(__file__).resolve().parent.parent.parent / "environments.toml",
        help="Path to environments.toml (default: auto-detected from repo root)",
    )
    parser.add_argument(
        "-t",
        "--token",
        default="",
        help="Docker Hub Personal Access Token (dckr_pat_xxx) or Organization Access Token (dckr_oat_xxx)",
    )
    parser.add_argument(
        "-a",
        "--archs",
        default=DEFAULT_ARCHS,
        help=f"Comma-separated required architectures (default: {DEFAULT_ARCHS})",
    )
    parser.add_argument(
        "-j",
        "--jobs",
        type=int,
        default=4,
        help="Number of concurrent API requests (default: 4)",
    )
    parser.add_argument(
        "--format",
        choices=["text", "json"],
        default="text",
        help="Output format (default: text)",
    )
    return parser.parse_args()


def create_session(token: str) -> requests.Session:
    session = requests.Session()
    session.headers["User-Agent"] = "vulhub-arch-checker/1.0"
    if token:
        # Docker Hub requires "JWT" scheme for PAT/OAT tokens, not "Bearer".
        # Using "Bearer" results in a 401 error.
        session.headers["Authorization"] = f"JWT {token}"
        print("Using authenticated Docker Hub API.", file=sys.stderr)
    else:
        print(
            "WARNING: No token provided, using anonymous API (rate limit ~100 req/6h).\n"
            "Use --token to provide a Docker Hub access token.\n",
            file=sys.stderr,
        )
    return session


def get_image_archs(session: requests.Session, repo: str, tag: str) -> set[str]:
    """Query Docker Hub for the architectures available for repo:tag."""
    url = f"{DOCKERHUB_API}/{repo}/tags/{tag}"
    for attempt in range(MAX_RETRIES):
        try:
            resp = session.get(url, timeout=30)
        except requests.RequestException as e:
            print(f"  Request error: {e}, retrying...", file=sys.stderr)
            time.sleep(RETRY_BACKOFF)
            continue

        if resp.status_code == 429:
            retry_after = int(resp.headers.get("Retry-After", RETRY_BACKOFF))
            print(f"  Rate limited, waiting {retry_after}s...", file=sys.stderr)
            time.sleep(retry_after)
            continue
        if resp.status_code == 404:
            return set()

        resp.raise_for_status()
        data = resp.json()
        return {
            img["architecture"]
            for img in data.get("images", [])
            if img.get("os") == "linux"
        }

    return set()


def check_image(
    session: requests.Session, image: str, required_archs: set[str]
) -> dict | None:
    """Check a single image. Returns a result dict if archs are missing, else None."""
    parts = image.split(":", 1)
    if len(parts) != 2:
        return None

    repo, tag = parts
    archs = get_image_archs(session, repo, tag)
    missing_archs = required_archs - archs

    if missing_archs:
        return {
            "image": image,
            "has": sorted(archs),
            "missing": sorted(missing_archs),
        }
    return None


def load_images(toml_path: Path) -> tuple[list[str], dict[str, list[str]]]:
    """Load environments.toml and return (sorted unique images, image->envs mapping)."""
    with open(toml_path, "rb") as f:
        data = tomllib.load(f)

    image_envs: dict[str, list[str]] = defaultdict(list)
    for env in data["environment"]:
        for image in env.get("dockerfile", {}):
            image_envs[image].append(env["path"])

    return sorted(image_envs.keys()), dict(image_envs)


def output_text(results: list[dict], image_envs: dict[str, list[str]], total: int):
    print(f"\n{'=' * 80}")
    print(f"Images missing multi-arch support: {len(results)} / {total}")
    print(f"{'=' * 80}\n")

    for r in results:
        has = ", ".join(r["has"]) if r["has"] else "NONE"
        needs = ", ".join(r["missing"])
        envs = image_envs.get(r["image"], [])
        print(f"Image: {r['image']}")
        print(f"  Has:     {has}")
        print(f"  Missing: {needs}")
        print(f"  Used by: {', '.join(envs)}")
        print()


def output_json(results: list[dict], image_envs: dict[str, list[str]], total: int):
    output = {
        "total": total,
        "missing_count": len(results),
        "images": [
            {**r, "environments": image_envs.get(r["image"], [])} for r in results
        ],
    }
    print(json.dumps(output, indent=2))


def main():
    args = parse_args()
    required_archs = {a.strip() for a in args.archs.split(",")}
    session = create_session(args.token)

    if not args.file.exists():
        print(f"Error: {args.file} not found.", file=sys.stderr)
        sys.exit(2)

    images, image_envs = load_images(args.file)
    total = len(images)
    print(
        f"Checking {total} unique images (archs: {', '.join(sorted(required_archs))})...\n",
        file=sys.stderr,
    )

    results: list[dict] = []
    checked = 0

    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        futures = {
            pool.submit(check_image, session, image, required_archs): image
            for image in images
        }
        for future in as_completed(futures):
            checked += 1
            image = futures[future]
            try:
                result = future.result()
            except Exception as e:
                print(
                    f"[{checked}/{total}] {image} ... ERROR: {e}", file=sys.stderr
                )
                continue

            if result:
                results.append(result)
                print(
                    f"[{checked}/{total}] {image} ... MISSING {result['missing']}",
                    file=sys.stderr,
                )
            else:
                print(f"[{checked}/{total}] {image} ... OK", file=sys.stderr)

    results.sort(key=lambda r: r["image"])

    if args.format == "json":
        output_json(results, image_envs, total)
    else:
        output_text(results, image_envs, total)

    sys.exit(1 if results else 0)


if __name__ == "__main__":
    main()
