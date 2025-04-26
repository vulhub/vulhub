# Java RMI Codebase Remote Code Execution

[中文版本(Chinese version)](README.zh-cn.md)

Java Remote Method Invocation (RMI) is used for remote procedure calls in Java. Under certain conditions, an RMI client can specify `java.rmi.server.codebase` to make the server load remote objects, leading to the execution of arbitrary Java bytecode on the server.

References:

- <https://docs.oracle.com/javase/7/docs/technotes/guides/rmi/codebase.html>
- <https://paper.seebug.org/1091/>

## Environment Setup

Execute the following commands to compile and start the RMI Registry and server:

```
docker compose build
docker compose run -e RMIIP=your-ip -p 1099:1099 -p 64000:64000 rmi
```

Replace `your-ip` with your server's IP address. The client will use this IP to connect to the server.

After startup, the RMI Registry will be listening on port 1099.

## Vulnerability Reproduction

To be completed.
