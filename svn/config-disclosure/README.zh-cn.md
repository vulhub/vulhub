# SVN信息泄露漏洞

SVN（subversion）是常见的版本控制软件，在使用svn checkout拉取代码时，会在环境中保留`.svn/`目录，用于保存对于版本库的被指信息。

部分网页代码使用svn checkout进行部署，因此可以使用利用`.svn/`下的配置文件还原所有源代码，造成源码泄露。

参考链接：

- <https://www.zaproxy.org/docs/alerts/42/>

## 环境搭建

执行如下命令启动一个包含svn泄露漏洞的web应用：

```
docker compose up -d
```

环境启动后，访问`http://your-ip:8080`即可查看首页。

## 漏洞复现

直接请求svn配置文件，观察其是否返回文件内容
```
curl http://your-ip:8080/.svn/entries
```

![verified](./verified.png)
