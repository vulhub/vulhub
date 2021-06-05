# 心脏出血漏洞（CVE-2014-0160）

心脏出血是OpenSSL库中的一个内存漏洞，攻击者利用这个漏洞可以服务到目标进程内存信息，如其他人的Cookie等敏感信息。

参考链接：

- https://heartbleed.com/
- https://filippo.io/Heartbleed

## 环境搭建

运行如下命令启动一个使用了OpenSSL 1.0.1c的Nginx服务器：

```
docker-compose up -d
```

环境启动后，访问`https://your-ip`即可查看到hello页面。

## 漏洞复现

访问<https://filippo.io/Heartbleed>进行在线检测：

![](1.png)

Python运行[ssltest.py](ssltest.py)，拿到敏感数据（Cookie）：

![](2.png)
