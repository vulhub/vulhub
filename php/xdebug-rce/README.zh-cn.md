# PHP XDebug 远程调试导致代码执行漏洞

XDebug 是一个用于调试 PHP 代码的扩展。当启用远程调试模式并设置 `remote_connect_back = 1` 时，攻击者可以通过利用调试协议在目标服务器上执行任意 PHP 代码。

当 XDebug 配置如下时可能存在漏洞：

```ini
xdebug.remote_connect_back = 1
xdebug.remote_enable = 1
```

参考链接：

- <https://ricterz.me/posts/Xdebug%3A%20A%20Tiny%20Attack%20Surface>
- <https://xdebug.org>

## 环境搭建

执行如下命令编译并启动漏洞环境：

```
docker compose build
docker compose up -d
```

环境启动后，访问 `http://your-ip:8080/` 可以看到一个简单的 phpinfo 页面。在 PHP 配置部分可以验证 XDebug 已启用并配置了远程调试功能。

## 漏洞复现

由于漏洞需要使用 DBGp 协议与目标服务器通信，所以无法仅使用 HTTP 协议复现漏洞。

Vulhub 提供了一个[漏洞利用脚本](exp.py)，可以在目标服务器上执行任意 PHP 代码：

```bash
# 需要 Python 3 和 requests 库
python3 exp.py -t http://127.0.0.1:8080/index.php -c 'shell_exec("id");'
```

成功利用漏洞后将执行命令并返回输出：

![](1.png)

### 重要说明

漏洞利用过程涉及反向连接：

1. 利用脚本监听 9000 端口（可通过 `-l` 参数配置）
2. 当目标访问带有 `XDEBUG_SESSION_START=phpstorm` 参数的 URL 时，XDebug 会尝试连接回访问者的 IP
3. 你需要有一个公网 IP 地址或与目标在同一网络中才能接收到连接
