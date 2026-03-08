<!-- markdownlint-disable first-line-heading -->
<p align="center">
  <a href="https://vulhub.org" target="_blank"><img src=".github/assets/banner.png" alt="Vulhub" height="auto" /></a>
</p>

<p align="center">
  <a href="https://discord.gg/bQCpZEK" target="_blank"><img src="https://img.shields.io/discord/485505185167179778.svg" alt="Chat on Discord"></a>
  <a href="https://github.com/sponsors/phith0n" target="_blank"><img src="https://img.shields.io/github/sponsors/phith0n?color=aqua" alt="GitHub Sponsors"></a>
  <a href="https://vulhub.org/environments" target="_blank"><img alt="Vulnerabilities count" src="https://img.shields.io/badge/dynamic/json?url=https://vulhub.org/api/statistic&query=%24.environments&label=vulnerabilities"></a>
  <img alt="GitHub language count" src="https://img.shields.io/github/languages/count/vulhub/vulhub?color=yellow">
  <a href="https://github.com/vulhub/vulhub/graphs/contributors" target="_blank"><img alt="GitHub contributors" src="https://img.shields.io/github/contributors-anon/vulhub/vulhub?color=orange"></a>
  <a href="https://github.com/vulhub/vulhub/blob/master/LICENSE" target="_blank"><img src="https://img.shields.io/github/license/vulhub/vulhub.svg" alt="GitHub"></a>
</p>

Vulhub是一个开源的、即开即用的漏洞靶场环境集合。无需Docker基础，只需一条命令即可快速启动用于安全研究、学习或演示的漏洞环境。

## 快速开始

**安装Docker**（以Ubuntu 24.04为例）：

```bash
curl -s https://get.docker.com/ | sh
systemctl start docker
```

其他操作系统请参考[Docker官方文档](https://docs.docker.com/)。

虽然所有Vulhub环境都基于Docker compose制作，但你不再需要安装独立的docker-compose，而是使用Docker自带的compose命令来启动Vulhub环境。

**下载并解压Vulhub：**

```bash
git clone --depth 1 https://github.com/vulhub/vulhub
```

**启动一个漏洞环境：**

```bash
cd vulhub/langflow/CVE-2025-3248  # Example: enter a vulnerability directory
docker compose up -d
```

每个环境目录下都包含详细的**README**，请参阅以了解复现步骤和使用说明。

**测试完成后清理环境：**

```bash
docker compose down -v
```

> [!NOTE]
>
> - 推荐使用至少1GB内存的VPS或虚拟机
> - 文档中的`your-ip`指你的主机/VPS IP，不是Docker容器内部IP
> - 请确保Docker有权限访问当前目录下所有文件，避免权限错误
> - 部分环境可能不支持ARM架构，详见[常见问题](#常见问题)
> - **所有环境仅供测试与学习，严禁用于生产环境！**

## 常见问题

**中国大陆拉取镜像失败**

Docker Hub 在中国大陆可能无法访问，可以使用镜像站加速，或使用境外VPS进行测试。

**Apple Silicon（M系列芯片）Mac 上环境启动失败**

大部分Vulhub环境可以在M系列芯片的Docker Desktop上直接运行。如果遇到启动失败，可以尝试指定平台后再运行：

```bash
export DOCKER_DEFAULT_PLATFORM=linux/amd64
docker compose up -d
```

**Kali Linux 上环境运行失败**

部分环境在Kali Linux上运行失败，可能是因为 `ulimit nofile` 设置过低，请参考[FAQ](https://vulhub.org/documentation/faq)进行修复。

如果遇到无法解决的问题，欢迎到社区寻求帮助：

- [Discord](https://discord.gg/bQCpZEK)
- [X (Twitter)](https://x.com/vulhub)

## 贡献

欢迎贡献！请阅读[贡献指南](CONTRIBUTING.md)了解详情。

感谢所有贡献者：

[![](https://opencollective.com/vulhub/contributors.svg?width=890&button=false)](https://github.com/vulhub/vulhub/graphs/contributors)

## 合作伙伴

我们的合作伙伴与用户：

<p>
  <a href="https://www.wangan.com/vulhub" target="_blank"><img src="https://vulhub.org/sponsor/wangan.png" width="200"></a>
  <a href="https://www.cvebase.com" target="_blank"><img src="https://vulhub.org/sponsor/cvebase.png" width="200"></a>
  <a href="https://www.huoxian.cn" target="_blank"><img src="https://vulhub.org/sponsor/huoxian.png" width="200"></a>
  <a href="https://www.chaitin.cn" target="_blank"><img src="https://vulhub.org/sponsor/chaitin.png" width="200"></a>
  <a href="https://xianzhi.aliyun.com/" target="_blank"><img src="https://vulhub.org/sponsor/aliyun.svg" width="200"></a>
</p>

欢迎通过[GitHub Sponsor](https://github.com/sponsors/phith0n)、[OpenCollective](https://opencollective.com/vulhub#backer)或[Patreon](https://www.patreon.com/bePatron?u=12677520)支持Vulhub 🙏

<p>
  <a href="https://github.com/sponsors/phith0n"><img src="https://opencollective.com/vulhub/backers.svg?width=138"></a>
  <a href="https://github.com/sponsors/phith0n"><img src="https://opencollective.com/vulhub/sponsors.svg?width=138"></a>
</p>

更多[捐助方式](https://vulhub.org)。

## License

Vulhub 遵循 MIT License，详见[LICENSE](LICENSE)。
