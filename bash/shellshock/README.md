# Shellshock 破壳漏洞（CVE-2014-6271）

编译运行：

```
docker-compose up -d
```

访问`http://your-ip:8080/`，可见有两个文件：

 - safe.cgi
 - victim.cgi

其中safe.cgi是最新版bash生成的页面，victim.cgi是bash4.3生成的页面。

带上payload访问victim.cgi，命令成功被执行：

![](1.png)

同样的数据包访问safe.cgi，不受影响：

![](2.png)