# PHPMailer 任意文件读取漏洞（CVE-2017-5223）

## 漏洞环境

在当前目录下创建文件`.env`，内容如下（将其中的配置值修改成你的smtp服务器、账户、密码）：

```
SMTP_SERVER=smtp.example.com
SMTP_PORT=587
SMTP_EMAIL=your_email@example.com
SMTP_PASSWORD=secret
SMTP_SECURE=tls
```

其中，`SMTP_SECURE`是SMTP加密方式，可以填写none、ssl或tls。

