#!/usr/bin/env python3
"""Check Docker images in environments.toml for multi-arch support.

Finds all environments whose Docker images are missing amd64 or arm64 variants.
Designed to run as a CI checker in GitHub Actions.

Uses the Docker Registry v2 API to check manifest lists. Authenticates via
auth.docker.io token endpoint. Team/Pro/Business accounts have unlimited
pull rate limits on the Registry API, unlike the Hub REST API which has a
per-IP 180 req/min limit regardless of account type.

Usage:
    python3 check_image_arch.py --username vulhub --token dckr_oat_xxx
    python3 check_image_arch.py --username user --token dckr_pat_xxx --format json
"""

import argparse
import json
import signal
import sys
import threading
import tomllib
import time
from collections import defaultdict
from concurrent.futures import FIRST_COMPLETED, ThreadPoolExecutor, wait
from pathlib import Path

import requests

AUTH_URL = "https://auth.docker.io/token"
REGISTRY_URL = "https://registry-1.docker.io/v2"
DEFAULT_ARCHS = "amd64,arm64"
MAX_RETRIES = 3
RETRY_BACKOFF = 5

MANIFEST_LIST_TYPES = [
    "application/vnd.docker.distribution.manifest.list.v2+json",
    "application/vnd.oci.image.index.v1+json",
]
MANIFEST_ACCEPT = ", ".join(
    MANIFEST_LIST_TYPES
    + [
        "application/vnd.docker.distribution.manifest.v2+json",
        "application/vnd.oci.image.manifest.v1+json",
    ]
)

# Event to signal all threads to stop; set on Ctrl+C.
_shutdown = threading.Event()


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
        "-u",
        "--username",
        default="",
        help="Docker Hub username (or org name for OAT)",
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
        default=8,
        help="Number of concurrent API requests (default: 8)",
    )
    parser.add_argument(
        "--format",
        choices=["text", "json"],
        default="text",
        help="Output format (default: text)",
    )
    return parser.parse_args()


class RegistryClient:
    """Docker Registry v2 API client with auth.docker.io token caching."""

    def __init__(self, username: str = "", token: str = ""):
        self.username = username
        self.token = token
        self.session = requests.Session()
        self.session.headers["User-Agent"] = "vulhub-arch-checker/1.0"
        self._token_cache: dict[str, tuple[str, float]] = {}
        self._lock = threading.Lock()

    def _get_registry_token(self, repo: str) -> str:
        """Get a bearer token for the given repo, with caching."""
        with self._lock:
            if repo in self._token_cache:
                token, expires = self._token_cache[repo]
                if time.time() < expires:
                    return token

        params = {
            "service": "registry.docker.io",
            "scope": f"repository:{repo}:pull",
        }
        auth = None
        if self.username and self.token:
            auth = (self.username, self.token)

        resp = self.session.get(AUTH_URL, params=params, auth=auth, timeout=30)
        resp.raise_for_status()
        data = resp.json()
        token = data["token"]
        expires_in = data.get("expires_in", 300)
        with self._lock:
            self._token_cache[repo] = (token, time.time() + expires_in - 60)
        return token

    def get_image_archs(self, repo: str, tag: str) -> set[str]:
        """Get the set of linux architectures available for repo:tag."""
        for attempt in range(MAX_RETRIES):
            if _shutdown.is_set():
                return set()

            try:
                token = self._get_registry_token(repo)
                url = f"{REGISTRY_URL}/{repo}/manifests/{tag}"
                headers = {
                    "Authorization": f"Bearer {token}",
                    "Accept": MANIFEST_ACCEPT,
                }
                resp = self.session.get(url, headers=headers, timeout=30)
            except requests.RequestException as e:
                print(f"  Request error: {e}, retrying...", file=sys.stderr)
                _shutdown.wait(RETRY_BACKOFF)
                continue

            if resp.status_code == 429:
                retry_after = int(resp.headers.get("Retry-After", RETRY_BACKOFF))
                print(f"  Rate limited, waiting {retry_after}s...", file=sys.stderr)
                _shutdown.wait(retry_after)
                continue
            if resp.status_code == 404:
                return set()
            if resp.status_code == 401:
                with self._lock:
                    self._token_cache.pop(repo, None)
                _shutdown.wait(1)
                continue

            resp.raise_for_status()
            data = resp.json()
            media_type = data.get("mediaType", resp.headers.get("Content-Type", ""))

            if media_type in MANIFEST_LIST_TYPES:
                return {
                    m["platform"]["architecture"]
                    for m in data.get("manifests", [])
                    if m.get("platform", {}).get("os") == "linux"
                }

            # Single manifest — fetch config blob to determine architecture.
            config_digest = data.get("config", {}).get("digest", "")
            if config_digest:
                return self._get_arch_from_config(repo, config_digest, token)

            return set()

        return set()

    def _get_arch_from_config(
        self, repo: str, digest: str, token: str
    ) -> set[str]:
        """Fetch the image config blob to extract its architecture."""
        url = f"{REGISTRY_URL}/{repo}/blobs/{digest}"
        headers = {"Authorization": f"Bearer {token}"}
        try:
            resp = self.session.get(url, headers=headers, timeout=30)
            if resp.ok:
                arch = resp.json().get("architecture", "")
                return {arch} if arch else set()
        except requests.RequestException:
            pass
        return set()


def check_image(
    client: RegistryClient, image: str, required_archs: set[str]
) -> dict | None:
    """Check a single image. Returns a result dict if archs are missing, else None."""
    parts = image.split(":", 1)
    if len(parts) != 2:
        return None

    repo, tag = parts
    archs = client.get_image_archs(repo, tag)
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

    if args.username and args.token:
        print(
            f"Authenticating as '{args.username}' via Docker Registry API.",
            file=sys.stderr,
        )
    else:
        print(
            "WARNING: No credentials provided, using anonymous Registry API.\n"
            "Anonymous rate limit is ~100 pulls/6h. Use --username and --token to authenticate.\n",
            file=sys.stderr,
        )

    client = RegistryClient(username=args.username, token=args.token)

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

    # First Ctrl+C: set shutdown event so threads exit gracefully.
    # Second Ctrl+C: restore default handler to force-kill immediately.
    def _handle_sigint(signum, frame):
        _shutdown.set()
        signal.signal(signal.SIGINT, signal.SIG_DFL)
        print(
            "\nInterrupted, shutting down (press Ctrl+C again to force quit)...",
            file=sys.stderr,
        )

    signal.signal(signal.SIGINT, _handle_sigint)

    try:
        with ThreadPoolExecutor(max_workers=args.jobs) as pool:
            future_to_image = {
                pool.submit(check_image, client, image, required_archs): image
                for image in images
            }
            pending = set(future_to_image)

            while pending and not _shutdown.is_set():
                done, pending = wait(
                    pending, timeout=0.5, return_when=FIRST_COMPLETED
                )
                for future in done:
                    checked += 1
                    image = future_to_image[future]
                    try:
                        result = future.result()
                    except Exception as e:
                        print(
                            f"[{checked}/{total}] {image} ... ERROR: {e}",
                            file=sys.stderr,
                        )
                        continue

                    if result:
                        results.append(result)
                        print(
                            f"[{checked}/{total}] {image} ... MISSING {result['missing']}",
                            file=sys.stderr,
                        )
                    else:
                        print(
                            f"[{checked}/{total}] {image} ... OK", file=sys.stderr
                        )

            if _shutdown.is_set():
                for f in pending:
                    f.cancel()
    finally:
        signal.signal(signal.SIGINT, signal.SIG_DFL)

    if _shutdown.is_set():
        print("Aborted.", file=sys.stderr)
        sys.exit(130)

    results.sort(key=lambda r: r["image"])

    if args.format == "json":
        output_json(results, image_envs, total)
    else:
        output_text(results, image_envs, total)

    sys.exit(1 if results else 0)


if __name__ == "__main__":
    main()
