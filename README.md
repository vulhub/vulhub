# Vulhub - Docker-Compose files for vulnerabilities environment
[![Backers on Open Collective](https://opencollective.com/vulhub/backers/badge.svg)](#backers)
 [![Sponsors on Open Collective](https://opencollective.com/vulhub/sponsors/badge.svg)](#sponsors) 

Vulhub是一个面向大众的开源漏洞靶场，无需docker知识，简单执行两条命令即可编译、运行一个完整的漏洞靶场镜像。

在ubuntu16.04下安装docker/docker-compose:

## Installation

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
# 拉取项目
git clone https://github.com/vulhub/vulhub.git
cd vulhub

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
docker-compose down
```

本项目每个漏洞环境均附带文档，建议你购买1G内存的vps搭建漏洞测试环境，文档中所说的`your-ip`均指你的vps的ip地址，如果你是用虚拟机搭建测试环境，是指你的虚拟机IP，而不是docker容器内部的IP，请不要混淆。

**本项目中所有环境仅用于测试，不可作为生产环境使用！**

## Notice

注意事项：

1. 为防止出现权限错误，最好使用root用户执行docker和docker-compose命令
2. docker部分镜像不支持在ARM等架构的机器上运行

## Contribution

本项目依赖于docker，在编译及运行过程中出现的任意异常都是docker以及相关程序抛出的，请先自行查找错误原因。如果确定是因为Dockerfile编写错误（或vulhub中代码错误）导致的，再提交issue。更多说明请[这篇文档](https://github.com/phith0n/vulhub/wiki/%E7%BC%96%E8%AF%91%E5%A4%B1%E8%B4%A5%E7%9A%84%E5%8E%9F%E5%9B%A0)，希望可以对你有所帮助。

致谢列表：[Contributors List](contributors.md)

### Contributors

This project exists thanks to all the people who contribute. 
<a href="graphs/contributors"><img src="https://opencollective.com/vulhub/contributors.svg?width=890&button=false" /></a>


### Backers

Thank you to all our backers! 🙏 [[Become a backer](https://opencollective.com/vulhub#backer)]

<a href="https://opencollective.com/vulhub#backers" target="_blank"><img src="https://opencollective.com/vulhub/backers.svg?width=890"></a>


### Sponsors

Support this project by becoming a sponsor. Your logo will show up here with a link to your website. [[Become a sponsor](https://opencollective.com/vulhub#sponsor)]

<a href="https://opencollective.com/vulhub/sponsor/0/website" target="_blank"><img src="https://opencollective.com/vulhub/sponsor/0/avatar.svg"></a>
<a href="https://opencollective.com/vulhub/sponsor/1/website" target="_blank"><img src="https://opencollective.com/vulhub/sponsor/1/avatar.svg"></a>
<a href="https://opencollective.com/vulhub/sponsor/2/website" target="_blank"><img src="https://opencollective.com/vulhub/sponsor/2/avatar.svg"></a>
<a href="https://opencollective.com/vulhub/sponsor/3/website" target="_blank"><img src="https://opencollective.com/vulhub/sponsor/3/avatar.svg"></a>
<a href="https://opencollective.com/vulhub/sponsor/4/website" target="_blank"><img src="https://opencollective.com/vulhub/sponsor/4/avatar.svg"></a>
<a href="https://opencollective.com/vulhub/sponsor/5/website" target="_blank"><img src="https://opencollective.com/vulhub/sponsor/5/avatar.svg"></a>
<a href="https://opencollective.com/vulhub/sponsor/6/website" target="_blank"><img src="https://opencollective.com/vulhub/sponsor/6/avatar.svg"></a>
<a href="https://opencollective.com/vulhub/sponsor/7/website" target="_blank"><img src="https://opencollective.com/vulhub/sponsor/7/avatar.svg"></a>
<a href="https://opencollective.com/vulhub/sponsor/8/website" target="_blank"><img src="https://opencollective.com/vulhub/sponsor/8/avatar.svg"></a>
<a href="https://opencollective.com/vulhub/sponsor/9/website" target="_blank"><img src="https://opencollective.com/vulhub/sponsor/9/avatar.svg"></a>



## License

Vulhub is released under the [GPL-3.0 license](LICENSE).
