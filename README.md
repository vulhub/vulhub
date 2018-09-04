# Vulhub - Docker-Compose files for vulnerabilities environment

[中文版本(Chinese version)](README.zh-cn.md)

[![Docker Stars](https://img.shields.io/badge/docker%20stars-20k-blue.svg)](https://hub.docker.com/u/vulhub/) [![GitHub](https://img.shields.io/github/license/vulhub/vulhub.svg)](https://github.com/vulhub/vulhub/blob/master/LICENSE)
 [![Chat on Discord](https://img.shields.io/discord/485505185167179778.svg)](https://discord.gg/GhMB3Z) [![Backers and sponors on Patreon](https://img.shields.io/badge/sponsor-patreon-73d6a1.svg)](https://www.patreon.com/phith0n) [![Backers and sponors on Opencollective](https://img.shields.io/badge/backer-opencollective-f89a76.svg)](https://www.patreon.com/phith0n)

Vulhub is an open source vulnerability lab for public. No docker knowledge is required, just execute two simple commands and you have a vulnerable environment.

## Installation

Install the docker/docker-compose on ubuntu16.04:

```bash
# Install pip
curl -s https://bootstrap.pypa.io/get-pip.py | python3

# Install the latest version docker
curl -s https://get.docker.com/ | sh

# Run docker service
service docker start

# Install docker compose
pip install docker-compose 
```

The installation steps of docker and docker-compose for others operating system might be slightly different, please refer to the docker documentation for details.

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

There is a **README** document in each environment directory, please read this file for vulnerability/environment testing.

After the test, delete the entire environment with the following command.
```
docker-compose down -v
```

It is recommended to use a VPS of at least 1G memory to build a vulnerability environment.It is recommended to use vps with 1G or above memory to build a vulnerability environment.The `your-ip` mentioned in the document refers to the IP address of your vps. If you are using a virtual machine, it refers to your virtual machine IP, not the IP inside the docker container, please don't confuse it.

**All environments in this project are for testing only, don't use it as production environment！**

## Notice

1. To prevent permission errors, it is best to use the root user to execute the docker and docker-compose commands.
2. Some docker mirror does not support running on ARM machine.
## Contribution

This project relies on docker. So any error during compilation and running are thrown by docker and related programs. Please find the cause of the error by yourself first. If it is determined that the dockerfile is written incorrectly (or the code is wrong in vulhub), then submit the issue.More details please 👉[Common reasons for compilation failure](https://github.com/phith0n/vulhub/wiki/%E7%BC%96%E8%AF%91%E5%A4%B1%E8%B4%A5%E7%9A%84%E5%8E%9F%E5%9B%A0), hope it can help you.

For more question, please contact:

- [Discord](https://discord.gg/GhMB3Z)
- [Twitter](https://twitter.com/vulhub)

Thanks for the following contributors:

[![](https://opencollective.com/vulhub/contributors.svg?width=890&button=false)](https://github.com/vulhub/vulhub/graphs/contributors)

More contributors：[Contributors List](contributors.md)

## Backer and Sponsor

Sponsor:

<p>
  <a href="https://www.chaitin.cn" target="_blank"><img src="https://vulhub.org/img/sponsor/chaitin.png" width="200"></a>
  <a href="https://bysec.io/" target="_blank"><img src="https://vulhub.org/img/sponsor/bysec.png" width="200"></a>
  <a href="https://xianzhi.aliyun.com/" target="_blank"><img src="https://vulhub.org/img/sponsor/aliyun.svg" width="200"></a>
</p>

Sponsor vulhub on patreon 🙏 

<a href="https://www.patreon.com/bePatron?u=12677520"><img src="https://vulhub.org/img/sponsor/patreon.png" width="150"></a>

Sponsor vulhub on opencollective 🙏 

<p>
  <a href="https://opencollective.com/vulhub#backer"><img src="https://opencollective.com/vulhub/backers.svg?width=138"></a>
  <a href="https://opencollective.com/vulhub#sponsor"><img src="https://opencollective.com/vulhub/sponsors.svg?width=138"></a>
</p>

More[Donate](http://vulhub.org/#/docs/donate/)。

## License

Vulhub is licensed under the MIT License. See [LICENSE](LICENSE) for the full license text.
