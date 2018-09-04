# OpenSSL Heartbleed Vulnerability (CVE-2014-0160)

[中文版本(Chinese version)](README.zh-cn.md)

build and start with：

```
docker-compose build
docker-compose up -d
```

visit `https://filippo.io/Heartbleed` to check：

![](1.png)

Python2 execute [ssltest.py](ssltest.py)，get sensitive data (Cookie)：

![](2.png)
