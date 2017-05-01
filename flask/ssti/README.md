# Flask（Jinja2） 服务端模板注入漏洞

## 原理

参考文章：

 - https://www.blackhat.com/docs/us-15/materials/us-15-Kettle-Server-Side-Template-Injection-RCE-For-The-Modern-Web-App-wp.pdf
 - http://rickgray.me/2016/02/24/use-python-feature-to-execute-arbitrary-codes-in-jinja2-templates.html

## 测试

编译及运行测试环境：

```
docker-compose build
docker-compose up -d
```

