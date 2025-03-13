<!-- markdownlint-disable first-line-heading -->
<p align="center">
  <img src=".github/assets/banner.png" alt="Vulhub" height="auto" />
  <p align="center">
    <a href="https://github.com/vulhub/vulhub/blob/master/LICENSE">
      <img src="https://img.shields.io/github/license/vulhub/vulhub.svg" alt="GitHub">
    </a>
    <a href="https://discord.gg/bQCpZEK">
      <img src="https://img.shields.io/discord/485505185167179778.svg" alt="Chat on Discord">
    </a>
    <a href="https://www.patreon.com/phith0n">
      <img src="https://img.shields.io/badge/sponsor-patreon-73d6a1.svg" alt="Backers and sponors on Patreon">
    </a>
    <a href="https://opencollective.com/vulhub#backer">
      <img src="https://img.shields.io/badge/backer-opencollective-f89a76.svg" alt="Backers and sponors on Opencollective">
    </a>
  </p>
</p>

Vulhub是一个面向大众的开源漏洞靶场，无需docker知识，简单执行一条命令即可编译、运行一个完整的漏洞靶场镜像。

## Installation

在Ubuntu 22.04下安装docker:

```bash
# 安装pip
curl -s https://bootstrap.pypa.io/get-pip.py | python3

# 安装最新版docker
curl -s https://get.docker.com/ | sh

# 启动docker服务
systemctl start docker
```

注意，从2022年4月起，`docker compose`作为一个子命令被并入Docker中，成为[Docker Compose V2](https://www.docker.com/blog/announcing-compose-v2-general-availability/)，Python版本的docker-compose将在2023年6月以后被弃用，故后续Vulhub的使用不再需要安装额外的docker-compose，所有文档也会修改为使用`docker compose`子命令。

其他操作系统安装docker可能会有些许不同，请阅读Docker文档进行安装。

## Usage

```bash
# 下载项目
wget https://github.com/vulhub/vulhub/archive/master.zip -O vulhub-master.zip
unzip vulhub-master.zip
cd vulhub-master

# 进入某一个漏洞/环境的目录
cd flask/ssti

# 自动化编译环境
docker compose build

# 启动整个环境
docker compose up -d
```

每个环境目录下都有相应的说明文件，请阅读该文件，进行漏洞/环境测试。

测试完成后，删除整个环境

```
docker compose down -v
```

本项目每个漏洞环境均附带文档，建议你购买1G内存的vps搭建漏洞测试环境，文档中所说的`your-ip`均指你的vps的ip地址，如果你是用虚拟机搭建测试环境，是指你的虚拟机IP，而不是docker容器内部的IP，请不要混淆。

**本项目中所有环境仅用于测试，不可作为生产环境使用！**

## Notice

注意事项：

1. 为防止出现权限错误，请保证docker容器有权限访问当前目录下所有文件
2. Vulhub暂不支持在ARM等非x86架构的机器上运行

## Contribution

本项目依赖于docker，在编译及运行过程中出现的任意异常都是docker以及相关程序抛出的，请先自行查找错误原因。如果确定是因为Dockerfile编写错误（或vulhub中代码错误）导致的，再提交issue。更多说明请[这篇文档](https://github.com/phith0n/vulhub/wiki/%E7%BC%96%E8%AF%91%E5%A4%B1%E8%B4%A5%E7%9A%84%E5%8E%9F%E5%9B%A0)，希望可以对你有所帮助。

更多问题，可以用如下方式和我们联系：

- [Discord](https://discord.gg/bQCpZEK)
- [Twitter](https://twitter.com/vulhub)

感谢如下贡献者：

[![](https://opencollective.com/vulhub/contributors.svg?width=890&button=false)](https://github.com/vulhub/vulhub/graphs/contributors)

更多无法体现在Contributors里的贡献者：[Contributors List](contributors.md)

## Backer and Sponsor

赞助商：

<p>
  <a href="https://www.wangan.com/vulhub" target="_blank"><img src="https://vulhub.org/sponsor/wangan.png" width="200"></a>
  <a href="https://www.cvebase.com" target="_blank"><img src="https://vulhub.org/sponsor/cvebase.png" width="200"></a>
  <a href="https://www.huoxian.cn" target="_blank"><img src="https://vulhub.org/sponsor/huoxian.png" width="200"></a>
  <a href="https://www.chaitin.cn" target="_blank"><img src="https://vulhub.org/sponsor/chaitin.png" width="200"></a>
  <a href="https://xianzhi.aliyun.com/" target="_blank"><img src="https://vulhub.org/sponsor/aliyun.svg" width="200"></a>
</p>

在patreon上赞助Vulhub 🙏

<a href="https://www.patreon.com/bePatron?u=12677520"><img src="https://vulhub.org/sponsor/patreon.png" width="150"></a>

在opencollective上赞助Vulhub 🙏

<p>
  <a href="https://opencollective.com/vulhub#backer"><img src="https://opencollective.com/vulhub/backers.svg?width=138"></a>
  <a href="https://opencollective.com/vulhub#sponsor"><img src="https://opencollective.com/vulhub/sponsors.svg?width=138"></a>
</p>

更多[捐助途径](http://vulhub.org)。

## License

Vulhub is licensed under the MIT License. See [LICENSE](LICENSE) for the full license text.
