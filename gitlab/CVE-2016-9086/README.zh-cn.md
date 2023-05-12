# GitLab 任意文件读取漏洞（CVE-2016-9086）

GitLab是一款Ruby开发的Git项目管理平台。在8.9版本后添加的“导出、导入项目”功能，因为没有处理好压缩包中的软连接，已登录用户可以利用这个功能读取服务器上的任意文件。

参考链接：

- https://about.gitlab.com/releases/2016/11/02/cve-2016-9086-patches/
- https://hackerone.com/reports/178152
- http://paper.seebug.org/104/

## 测试环境

执行如下命令启动一个GitLab Community Server 8.13.1：

```
docker-compose up -d
```

环境运行后，访问`http://your-ip:8080`即可查看GitLab主页，其ssh端口为10022，默认管理员账号、密码是`root`、`vulhub123456`。

> 注意，请使用2G及以上内存的VPS或虚拟机运行该环境，实测1G内存的机器无法正常运行GitLab（运行后502错误）。

## 漏洞复现

注册并登录用户，新建一个项目，点击`GitLab export`：

![](2.png)

在导入页面，将[test.tar.gz](test.tar.gz)上传，将会读取到`/etc/passwd`文件内容：

![](1.png)
