# Vulhub - Docker-Compose files for vulnerabilities environment

[![GitHub](https://img.shields.io/github/license/vulhub/vulhub.svg)](https://github.com/vulhub/vulhub/blob/master/LICENSE)
 [![讨论社区](https://img.shields.io/badge/%E8%AE%A8%E8%AE%BA-%E7%A4%BE%E5%8C%BA-blue.svg)](https://www.wangan.com/vulhub) [![Chat on Discord](https://img.shields.io/discord/485505185167179778.svg)](https://discord.gg/GhMB3Z) [![Backers and sponors on Patreon](https://img.shields.io/badge/sponsor-patreon-73d6a1.svg)](https://www.patreon.com/phith0n) [![Backers and sponors on Opencollective](https://img.shields.io/badge/backer-opencollective-f89a76.svg)](https://opencollective.com/vulhub#backer)

Vulhub是一个面向大众的开源漏洞靶场，无需docker知识，简单执行两条命令即可编译、运行一个完整的漏洞靶场镜像。

## Installation

在ubuntu16.04下安装docker/docker-compose:

```bash
# 安装pip
curl -s https://bootstrap.pypa.io/get-pip.py | python3

# 安装最新版docker
curl -s https://get.docker.com/ | sh

# 启动docker服务
service docker start

# 安装compose
pip install docker-compose 
```

其他操作系统安装docker和docker-compose可能会有些许不同，请阅读Docker文档进行安装。

## Usage

```bash
# 下载项目
wget https://github.com/vulhub/vulhub/archive/master.zip -O vulhub-master.zip
unzip vulhub-master.zip
cd vulhub-master

# 进入某一个漏洞/环境的目录
cd flask/ssti

# 自动化编译环境
docker-compose build

# 启动整个环境
docker-compose up -d
```

每个环境目录下都有相应的说明文件，请阅读该文件，进行漏洞/环境测试。

测试完成后，删除整个环境

```
docker-compose down -v
```

本项目每个漏洞环境均附带文档，建议你购买1G内存的vps搭建漏洞测试环境，文档中所说的`your-ip`均指你的vps的ip地址，如果你是用虚拟机搭建测试环境，是指你的虚拟机IP，而不是docker容器内部的IP，请不要混淆。

**本项目中所有环境仅用于测试，不可作为生产环境使用！**

## Notice

注意事项：

1. 为防止出现权限错误，最好使用root用户执行docker和docker-compose命令
2. docker部分镜像不支持在ARM等架构的机器上运行

## Contribution

本项目依赖于docker，在编译及运行过程中出现的任意异常都是docker以及相关程序抛出的，请先自行查找错误原因。如果确定是因为Dockerfile编写错误（或vulhub中代码错误）导致的，再提交issue。更多说明请[这篇文档](https://github.com/phith0n/vulhub/wiki/%E7%BC%96%E8%AF%91%E5%A4%B1%E8%B4%A5%E7%9A%84%E5%8E%9F%E5%9B%A0)，希望可以对你有所帮助。

更多问题，可以用如下方式和我们联系：

- [讨论社区](https://www.wangan.com/vulhub)
- [Discord](https://discord.gg/GhMB3Z)
- [Twitter](https://twitter.com/vulhub)

感谢如下贡献者：

[![](https://opencollective.com/vulhub/contributors.svg?width=890&button=false)](https://github.com/vulhub/vulhub/graphs/contributors)

更多无法体现在Contributors里的贡献者：[Contributors List](contributors.md)

## Backer and Sponsor

赞助商：

<p>
  <a href="https://www.chaitin.cn" target="_blank"><img src="https://vulhub.org/img/sponsor/chaitin.png" width="200"></a>
  <a href="https://xianzhi.aliyun.com/" target="_blank"><img src="https://vulhub.org/img/sponsor/aliyun.svg" width="200"></a>
  <a href="https://www.didiyun.com/?channel=14196" target="_blank"><img src="https://vulhub.org/img/sponsor/didi.png" width="200"></a>
</p>

在patreon上赞助vulhub 🙏 

<a href="https://www.patreon.com/bePatron?u=12677520"><img src="https://vulhub.org/img/sponsor/patreon.png" width="150"></a>

在opencollective上赞助vulhub 🙏 

<p>
  <a href="https://opencollective.com/vulhub#backer"><img src="https://opencollective.com/vulhub/backers.svg?width=138"></a>
  <a href="https://opencollective.com/vulhub#sponsor"><img src="https://opencollective.com/vulhub/sponsors.svg?width=138"></a>
</p>

更多[捐助途径](http://vulhub.org/#/docs/donate/)。

## License

Vulhub is licensed under the MIT License. See [LICENSE](LICENSE) for the full license text.
