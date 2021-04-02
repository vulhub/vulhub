<p align="center">
  <img src=".github/assets/logo.svg" alt="Vulhub" height="300" />
  <p align="center">
    <a href="https://github.com/vulhub/vulhub/blob/master/LICENSE">
      <img src="https://img.shields.io/github/license/vulhub/vulhub.svg" alt="GitHub">
    </a> 
    <a href="https://www.wangan.com/vulhub">
      <img src="https://img.shields.io/badge/Official-Community-blue.svg" alt="Official Community">
    </a>
    <a href="https://discord.gg/GhMB3Z">
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

Vulhub is an open-source collection of pre-built vulnerable docker environments. No pre-existing knowledge of docker is required, just execute two simple commands and you have a vulnerable environment.

[中文版本(Chinese version)](README.zh-cn.md)

## Installation

Install the docker/docker-compose on Ubuntu 20.04:

```bash
# Install pip
curl -s https://bootstrap.pypa.io/get-pip.py | python3

# Install the latest version docker
curl -s https://get.docker.com/ | sh

# Run docker service
systemctl start docker

# Install docker compose
pip install docker-compose
```

The installation steps of docker and docker-compose for other operating systems might be slightly different, please refer to the [docker documentation](https://docs.docker.com/) for details.

## Usage

```bash
# Download project
wget https://github.com/vulhub/vulhub/archive/master.zip -O vulhub-master.zip
unzip vulhub-master.zip
cd vulhub-master

# Enter the directory of vulnerability/environment
cd flask/ssti

# Compile environment
docker-compose build

# Run environment
docker-compose up -d
```

There is a **README** document in each environment directory, please read this file for vulnerability/environment testing and usage.

After the test, delete the environment with the following command.
```
docker-compose down -v
```

It is recommended to use a VPS of at least 1GB memory to build a vulnerability environment. The `your-ip` mentioned in the documentation refers to the IP address of your VPS. If you are using a virtual machine, it refers to your virtual machine IP, not the IP inside the docker container.

**All environments in this project are for testing purposes only and should not be used as a production environment!**

## Notice

1. To prevent permission errors, it is best to use the root user to execute the docker and docker-compose commands.
2. Some docker images do not support running on ARM machines.

## Contribution

This project relies on docker. So any error during compilation and running are thrown by docker and related programs. Please find the cause of the error by yourself first. If it is determined that the dockerfile is written incorrectly (or the code is wrong in vulhub), then submit the issue. More details please 👉[Common reasons for compilation failure](https://github.com/phith0n/vulhub/wiki/%E7%BC%96%E8%AF%91%E5%A4%B1%E8%B4%A5%E7%9A%84%E5%8E%9F%E5%9B%A0), hope it can help you.

For more question, please contact:

- [Chinese Community](https://www.wangan.com/vulhub)
- [Discord](https://discord.gg/GhMB3Z)
- [Twitter](https://twitter.com/vulhub)

Thanks for the following contributors:

[![](https://opencollective.com/vulhub/contributors.svg?width=890&button=false)](https://github.com/vulhub/vulhub/graphs/contributors)

More contributors：[Contributors List](contributors.md)

## Partner

Our Partners and users:

<p>
  <a href="https://www.wangan.com/vulhub" target="_blank"><img src="https://vulhub.org/img/sponsor/wangan.png" width="200"></a>
  <a href="https://www.cvebase.com" target="_blank"><img src="https://vulhub.org/img/sponsor/cvebase.png" width="200"></a>
  <a href="https://www.huoxian.cn" target="_blank"><img src="https://vulhub.org/img/sponsor/huoxian.png" width="200"></a>
  <a href="https://www.chaitin.cn" target="_blank"><img src="https://vulhub.org/img/sponsor/chaitin.png" width="200"></a>
  <a href="https://xianzhi.aliyun.com/" target="_blank"><img src="https://vulhub.org/img/sponsor/aliyun.svg" width="200"></a>
</p>

Sponsor vulhub on patreon 🙏

<a href="https://www.patreon.com/bePatron?u=12677520"><img src="https://vulhub.org/img/sponsor/patreon.png" width="150"></a>

Sponsor vulhub on opencollective 🙏

<p>
  <a href="https://opencollective.com/vulhub#backer"><img src="https://opencollective.com/vulhub/backers.svg?width=138"></a>
  <a href="https://opencollective.com/vulhub#sponsor"><img src="https://opencollective.com/vulhub/sponsors.svg?width=138"></a>
</p>

More [Donate](http://vulhub.org/#/docs/donate/).

## License

Vulhub is licensed under the MIT License. See [LICENSE](LICENSE) for the full license text.
