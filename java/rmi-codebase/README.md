# Java RMI codebase 远程代码执行漏洞

Java Remote Method Invocation 用于在Java中进行远程调用，在满足一定条件的情况下，RMI客户端通过指定`java.rmi.server.codebase`可以让服务端远程加载对象，进而加载远程java字节码执行任意代码。

## 漏洞环境

执行如下命令编译及启动RMI Registry和服务器：

```
docker-compose build
docker-compose run -e RMIIP=your-ip -p 1099:1099 -p 64000:64000 rmi
```

其中，`your-ip`是服务器IP，客户端会根据这个IP来连接服务器。

环境启动后，RMI Registry监听在1099端口。

## 漏洞复现

待完善。
