# ThinkPHP5 5.0.23 远程代码执行漏洞

ThinkPHP是一款运用极广的PHP开发框架。其5.0.23以前的版本中，获取method的方法中没有正确处理方法名，导致攻击者可以调用Request类任意方法并构造利用链，从而导致远程代码执行漏洞。

参考链接：

- https://github.com/top-think/framework/commit/4a4b5e64fa4c46f851b4004005bff5f3196de003

## 漏洞环境

执行如下命令启动一个默认的thinkphp 5.0.23环境：

```
docker-compose up -d
```

环境启动后，访问`http://your-ip:8080`即可看到默认的ThinkPHP启动页面。

## 漏洞复现

发送数据包`****`（考虑影响，暂不公开，知道Payload的同学可以自己测试）：

![](1.png)
