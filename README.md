# Vulhub - Some Docker-Compose files for vulnerabilities environment

Vulhub是一个面向大众的开源漏洞靶场，无需docker知识，简单执行两条命令即可编译、运行一个完整的漏洞靶场镜像。

在ubuntu16.04下安装docker/docker-compose:

```bash
# 安装pip
curl -s https://bootstrap.pypa.io/get-pip.py | python3

# 安装docker
apt-get update && apt-get install docker.io

# 启动docker服务
service docker start

# 安装compose
pip install docker-compose 
```

其他操作系统安装docker和docker-compose可能会有些许不同，如ubuntu14.04用默认源安装的docker可能是老版本，运行本项目会有不在预期中的BUG，所以最好参考docker官方推荐的安装方法（ https://docs.docker.com/engine/installation/linux/ubuntu/ ）使用官方源进行安装。

启动

```bash
# 拉取项目
git clone git@github.com:phith0n/vulhub.git
cd vulhub

# 进入某一个漏洞/环境的目录
cd nginx_php5_mysql

# 自动化编译环境
docker-compose build

# 启动整个环境
docker-compose up -d
```

每个环境目录下都有相应的说明文件，请阅读该文件，进行漏洞/环境测试。

测试完成后，删除整个环境

```
docker-compose down
```

如果你在编译的时候出现错误，不要盲目开issue，请阅读[这篇文档](https://github.com/phith0n/vulhub/wiki/%E7%BC%96%E8%AF%91%E5%A4%B1%E8%B4%A5%E7%9A%84%E5%8E%9F%E5%9B%A0)，希望可以对你有所帮助。

本项目每个漏洞环境均附带文档，建议你购买1G内存的vps搭建漏洞测试环境，文档中所说的`your-ip`均指你的vps的ip地址，如果你是用虚拟机搭建测试环境，是指你的虚拟机IP，而不是docker容器内部的IP，请不要混淆。

**本项目中所有环境仅用于测试，不可作为生成环境使用！**