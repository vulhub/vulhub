# 心脏出血漏洞（CVE-2014-0160）测试环境

编译运行：

```
docker-compose build
docker-compose up -d
```

访问`https://filippo.io/Heartbleed`进行在线检测：

![](1.png)

Python2运行[ssltest.py](ssltest.py)，拿到敏感数据（Cookie）：

![](2.png)