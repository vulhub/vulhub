# OpenSSL Heartbleed Vulnerability (CVE-2014-0160)

[中文版本(Chinese version)](README.zh-cn.md)

Build and run:

```
docker-compose build
docker-compose up -d
```

Visit `https://filippo.io/Heartbleed` to check：

![](1.png)

Run [ssltest.py](ssltest.py) with Python 2 to obtain sensitive data (Cookie)：

![](2.png)
