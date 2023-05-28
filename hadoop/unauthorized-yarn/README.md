# Hadoop YARN ResourceManager 未授权访问

## 原理

参考 http://archive.hack.lu/2016/Wavestone%20-%20Hack.lu%202016%20-%20Hadoop%20safari%20-%20Hunting%20for%20vulnerabilities%20-%20v1.0.pdf

## 测试环境

运行测试环境

```
docker compose up -d
```

环境启动后，访问`http://your-ip:8088`即可看到Hadoop YARN ResourceManager WebUI页面。

## 利用

利用方法和原理中有一些不同。在没有 hadoop client 的情况下，直接通过 REST API
 (https://hadoop.apache.org/docs/r2.7.3/hadoop-yarn/hadoop-yarn-site/ResourceManagerRest.html) 也可以提交任务执行。

利用过程如下：

1. 在本地监听等待反弹 shell 连接
1. 调用 New Application API 创建 Application
1. 调用 Submit Application API 提交

参考 [exp 脚本](exploit.py)
