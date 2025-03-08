# Java RMI Codebase 远程代码执行漏洞

Java Remote Method Invocation（RMI）是Java中用于远程过程调用的机制。在满足特定条件的情况下，RMI客户端可以通过指定`java.rmi.server.codebase`参数，使服务端加载远程对象，从而执行任意Java字节码。

参考链接：

- <https://docs.oracle.com/javase/7/docs/technotes/guides/rmi/codebase.html>
- <https://paper.seebug.org/1091/>

## 环境搭建

执行如下命令编译及启动RMI Registry和服务器：

```
docker compose build
docker compose run -e RMIIP=your-ip -p 1099:1099 -p 64000:64000 rmi
```

将`your-ip`替换为你的服务器IP地址，客户端将使用此IP连接服务器。

环境启动后，RMI Registry将监听在1099端口。

## 漏洞复现

待完善。
