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

Vulhub is an open-source collection of pre-built, ready-to-use vulnerable Docker environments. With just one command you can launch a vulnerable environment for security research, learning, or demonstration, no prior Docker experience required.

[中文版本(Chinese version)](README.zh-cn.md)

## Quick Start

**Install Docker** (example for Ubuntu 24.04):

```bash
# Install the latest version docker
curl -s https://get.docker.com/ | sh

# Run docker service
systemctl start docker
```

For other operating systems, see the [Docker documentation](https://docs.docker.com/).

Although all Vulhub environments are running based on Docker Compose, you no longer need to install docker-compose separately. Instead, you can use the built-in `docker compose` command to start Vulhub environments.

**Download and set up Vulhub:**

```bash
git clone --depth 1 https://github.com/vulhub/vulhub
```

**Launch a vulnerable environment:**

```bash
cd vulhub/langflow/CVE-2025-3248  # Example: enter a vulnerability directory
docker compose up -d
```

Each environment directory contains a detailed **README** with reproduction steps and usage instructions.

**Clean up after testing:**

```bash
docker compose down -v
```

> [!NOTE]
>
> - Use a VPS or VM with at least 1GB RAM for best results
> - The `your-ip` in documentation refers to your host/VPS IP, not the Docker container IP
> - Ensure Docker has permission to access all files in the current directory to avoid permission errors
> - Vulhub currently supports only x86 architectures (not ARM)
> - **All environments are for testing and educational purposes only. Do not use in production!**

## Contributing

If you encounter errors during build or runtime, please first check if they are caused by Docker or related dependencies. If you confirm an issue with a Dockerfile or Vulhub code, submit an issue. See [FAQ](https://vulhub.org/documentation/faq) for troubleshooting tips.

For questions, contact us:

- [Discord](https://discord.gg/bQCpZEK)
- [Twitter](https://twitter.com/vulhub)

Thanks to all [contributors](contributors.md):

[![](https://opencollective.com/vulhub/contributors.svg?width=890&button=false)](https://github.com/vulhub/vulhub/graphs/contributors)

## Partners

Our partners and users:

<p>
  <a href="https://www.wangan.com/vulhub" target="_blank"><img src="https://vulhub.org/sponsor/wangan.png" width="200"></a>
  <a href="https://www.cvebase.com" target="_blank"><img src="https://vulhub.org/sponsor/cvebase.png" width="200"></a>
  <a href="https://www.huoxian.cn" target="_blank"><img src="https://vulhub.org/sponsor/huoxian.png" width="200"></a>
  <a href="https://www.chaitin.cn" target="_blank"><img src="https://vulhub.org/sponsor/chaitin.png" width="200"></a>
  <a href="https://xz.aliyun.com/" target="_blank"><img src="https://vulhub.org/sponsor/aliyun.svg" width="200"></a>
</p>

Sponsor Vulhub on [GitHub Sponsor](https://github.com/sponsors/phith0n), [OpenCollective](https://opencollective.com/vulhub#backer), or [Patreon](https://www.patreon.com/bePatron?u=12677520) 🙏

<p>
  <a href="https://github.com/sponsors/phith0n"><img src="https://opencollective.com/vulhub/backers.svg?width=138"></a>
  <a href="https://github.com/sponsors/phith0n"><img src="https://opencollective.com/vulhub/sponsors.svg?width=138"></a>
</p>

More ways to [donate](https://vulhub.org/).

## License

Vulhub is licensed under the MIT License. See [LICENSE](LICENSE) for details.
