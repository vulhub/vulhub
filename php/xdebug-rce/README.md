# XDebug Remote Debugging Vulnerability (Code Execution)

XDebug is an extension to PHP for debugging PHP code. If the target has remote debug mode turned on, and set `remote_connect_back = 1`:

```
Xdebug.remote_connect_back = 1
Xdebug.remote_enable = 1
```

In this configuration, we access `http://target/index.php?XDEBUG_SESSION_START=phpstorm`, the XDebug of the target server will connect to the visitor's IP (or the address specified by the `X-Forwarded-For` header) and pass dbgp The protocol communicates with it, and we can execute arbitrary PHP code on the target server through the eval method provided in dbgp.

More instructions can be found at:

 - https://ricterz.me/posts/Xdebug%3A%20A%20Tiny%20Attack%20Surface
 - https://xdebug.org

## test environment

Compile and start the test environment

```
Docker-compose build
Docker-compose up -d
```

After the startup is complete, visit `http://your-ip:8080/` to find that the home page is a simple phpinfo, where you can find the xdebug configuration, visible remote debugging.

## Vulnerability

Because the dbgp protocol needs to communicate with the target server, the vulnerability cannot be reproduced with the http protocol.

I wrote a [vulnerability replay script] (exp.py), specifying the target web address, the PHP code to be executed:

```
#Request to use python3 and install the requests library
Python3 exp.py -t http://127.0.0.1:8080/index.php -c 'shell_exec('id');'
```

![](1.png)

**Important note: Because the communication is a reverse connection process, after exp.py starts, it will listen to the local 9000 port (can be specified by the -l parameter) and wait for XDebug to connect, so the server executing the script There must be an external network IP (or the same intranet as the target server). **