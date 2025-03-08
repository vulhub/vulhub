# XDebug Remote Debugging Code Execution

[中文版本(Chinese version)](README.zh-cn.md)

XDebug is a PHP extension used for debugging PHP code. When remote debugging mode is enabled with `remote_connect_back = 1`, an attacker can execute arbitrary PHP code on the target server by exploiting the debug protocol.

The vulnerability occurs when the following XDebug configuration is enabled:

```ini
xdebug.remote_connect_back = 1
xdebug.remote_enable = 1
```

With the above configuration, XDebug will attempt to connect back to the attacker's IP when a client visits `http://target/index.php?XDEBUG_SESSION_START=phpstorm` through the DBGp protocol, it provide a `eval` function that can be used to execute arbitrary PHP code.

References:

- <https://ricterz.me/posts/Xdebug%3A%20A%20Tiny%20Attack%20Surface>
- <https://xdebug.org>

## Environment Setup

Execute the following command to build and start the vulnerable environment:

```
docker compose build
docker compose up -d
```

After the environment is started, visit `http://your-ip:8080/` to see a simple phpinfo page. You can verify that XDebug is enabled and configured for remote debugging in the PHP configuration section.

## Vulnerability Reproduction

Since the vulnerability requires communication using the DBGp protocol with the target server, it cannot be reproduced using HTTP protocol alone.

A [proof-of-concept exploit script](exp.py) is provided that can execute arbitrary PHP code on the target server:

```bash
# Requires Python 3 and the requests library
python3 exp.py -t http://127.0.0.1:8080/index.php -c 'shell_exec("id");'
```

Successful exploitation will execute the command and return its output:

![](1.png)

### Important Notes

The exploitation process involves a reverse connection:

1. The exploit script listens on port 9000 (configurable with `-l` parameter)
2. When the target visits a URL with `XDEBUG_SESSION_START=phpstorm`, XDebug attempts to connect back to the visitor's IP
3. You have to have a public IP address or be in the same network as the target
