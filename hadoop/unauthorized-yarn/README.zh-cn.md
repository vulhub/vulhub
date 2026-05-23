# Hadoop YARN ResourceManager 未授权访问漏洞

Hadoop YARN（Yet Another Resource Negotiator）是 Apache Hadoop 的集群资源管理系统。YARN ResourceManager 中存在一个未授权访问漏洞，由于缺少访问控制，未经授权的用户可以在集群上提交并执行任意应用程序。

参考链接：

- <http://archive.hack.lu/2016/Wavestone%20-%20Hack.lu%202016%20-%20Hadoop%20safari%20-%20Hunting%20for%20vulnerabilities%20-%20v1.0.pdf>
- <https://hadoop.apache.org/docs/r2.7.3/hadoop-yarn/hadoop-yarn-site/ResourceManagerRest.html>

## 环境搭建

执行如下命令启动环境：

```
docker compose up -d
```

环境启动后，访问 `http://your-ip:8088` 即可看到 Hadoop YARN ResourceManager 的 Web 管理界面。

## 漏洞复现

漏洞利用方法与原始演示文稿中的方法略有不同。即使没有 Hadoop 客户端，也可以直接通过 REST API（https://hadoop.apache.org/docs/r2.7.3/hadoop-yarn/hadoop-yarn-site/ResourceManagerRest.html）提交任务执行。

漏洞利用过程如下：

1. 在本地机器上设置监听器，等待反弹 shell 连接
2. 调用 New Application API 创建应用程序
3. 调用 Submit Application API 提交恶意应用程序

具体实现请参考 [漏洞利用脚本](exploit.py)。
