# Erlang/OTP SSH 未授权远程代码执行漏洞（CVE-2025-32433）

Erlang/OTP SSH 是 Erlang/OTP 平台自带的 SSH 服务器组件。

在 Erlang/OTP SSH 服务端中发现了一个高危漏洞，攻击者可通过构造特定的 SSH 协议消息，在未认证的情况下远程执行任意系统命令。受影响版本包括 OTP-27.3.2 及更早版本、OTP-26.2.5.10 及更早版本，以及 OTP-25.3.2.19 及更早版本。

参考链接：

- <https://github.com/erlang/otp/security/advisories/GHSA-37cp-fgq5-7wc2>
- <https://github.com/erlang/otp/commit/6eef04130afc8b0ccb63c9a0d8650209cf54892f#diff-ceeb1aeeb602e1424c13d9da9383e0782f65869d6e64e015c194145b1a64edcd>
- <https://github.com/ProDefense/CVE-2025-32433>
- <https://datatracker.ietf.org/doc/html/rfc4254>

## 环境设置

执行如下命令启动一个用 Erlang/OTP 27.3.2 编写的 SSH 服务器：

```
docker compose up -d
```

环境启动后，容器内会运行 Erlang SSH 服务，监听 2222 端口，并映射到本地主机的 2222 端口。可通过 SSH 工具或漏洞利用脚本进行访问和测试。

## 漏洞复现

使用提供的 [exploit.py](exploit.py) 脚本即可复现漏洞：

```
python exploit.py -t 127.0.0.1 -p 2222 -c "touch /tmp/success"
```

![](1.png)

该脚本通过发送特制的 SSH_MSG_CHANNEL_REQUEST（消息编号 94）协议包，利用服务端处理缺陷，直接在未认证阶段执行任意命令。根据 RFC 4254，相关消息结构如下：

```
byte      SSH_MSG_CHANNEL_REQUEST
uint32    recipient channel
string    "exec"
boolean   want reply
string    command
```

命令执行成功后，进入容器即可看到 `/tmp/success` 文件已被创建：

![](2.png)
