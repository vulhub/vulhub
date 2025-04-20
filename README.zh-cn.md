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
      <img src="https://img.shields.io/badge/sponsor-patreon-73d6a1.svg" alt="Backers and sponsors on Patreon">
    </a>
    <a href="https://opencollective.com/vulhub#backer">
      <img src="https://img.shields.io/badge/backer-opencollective-f89a76.svg" alt="Backers and sponsors on Opencollective">
    </a>
  </p>
</p>

Vulhub是一个开源的、即开即用的漏洞靶场环境集合。无需Docker基础，只需两条命令即可快速启动用于安全研究、学习或演示的漏洞环境。

## 快速开始

1. **安装Docker**（以Ubuntu 22.04为例）：

```bash
curl -s https://get.docker.com/ | sh
systemctl start docker
```

其他操作系统请参考[Docker官方文档](https://docs.docker.com/)。

2. **下载并解压Vulhub：**

```bash
git clone --depth 1 https://github.com/vulhub/vulhub
```

3. **启动一个漏洞环境：**

```bash
cd langflow/CVE-2025-3248  # Example: enter a vulnerability directory
docker compose up -d
```

每个环境目录下都包含详细的**README**，请参阅以了解复现步骤和使用说明。

4. **测试完成后清理环境：**

```bash
docker compose down -v
```

> **注意：**
> - 推荐使用至少1GB内存的VPS或虚拟机。
> - 文档中的`your-ip`指你的主机/VPS IP，不是Docker容器内部IP。
> - **所有环境仅供测试与学习，严禁用于生产环境！**

## 注意事项

- 请确保Docker有权限访问当前目录下所有文件，避免权限错误。
- Vulhub目前仅支持x86架构（不支持ARM）。

## 贡献

如遇到编译或运行错误，请优先排查Docker及相关依赖问题。如确认是Dockerfile或Vulhub代码问题，请提交issue。常见问题可参考[FAQ](https://vulhub.org/documentation/faq)。

如有疑问，欢迎联系我们：

- [Discord](https://discord.gg/bQCpZEK)
- [Twitter](https://twitter.com/vulhub)

感谢所有[贡献者](contributors.md)：

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

欢迎通过[Patreon](https://www.patreon.com/bePatron?u=12677520)或[OpenCollective](https://opencollective.com/vulhub#backer)支持Vulhub 🙏

<p>
  <a href="https://www.patreon.com/bePatron?u=12677520"><img src="https://vulhub.org/sponsor/patreon.png" width="150"></a>
  <a href="https://opencollective.com/vulhub#backer"><img src="https://opencollective.com/vulhub/backers.svg?width=138"></a>
  <a href="https://opencollective.com/vulhub#sponsor"><img src="https://opencollective.com/vulhub/sponsors.svg?width=138"></a>
</p>

更多[捐助方式](http://vulhub.org)。

## License

Vulhub 遵循 MIT License，详见[LICENSE](LICENSE)。
