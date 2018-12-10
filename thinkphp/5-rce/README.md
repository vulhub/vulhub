# ThinkPHP5 5.0.22/5.1.29 远程代码执行漏洞

ThinkPHP是一款运用极广的PHP开发框架。其版本5中，由于没有正确处理控制器名，导致在网站没有开启强制路由的情况下（即默认情况下）可以执行任意方法，从而导致远程命令执行漏洞。

参考链接：

- http://www.thinkphp.cn/topic/60400.html
- http://www.thinkphp.cn/topic/60390.html

## 漏洞环境

运行ThinkPHP 5.0.20版本：

```
docker-compose up -d
```

环境启动后，访问`http://your-ip:8080`即可看到ThinkPHP默认启动页面。

## 漏洞复现

直接访问`***`（暂不公开），即可执行`phpinfo()`函数：

![image](https://ws2.sinaimg.cn/large/400f4ee7ly1fy1zh7yiewj21290kptcu.jpg)