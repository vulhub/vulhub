# Vulhub - Docker-Compose files for vulnerabilities environment

Vulhub is an open source vulnerability target for the general public. Without the knowledge of docker, you can compile and run a complete vulnerability target image by simply executing two commands.

Install docker/docker-compose under ubuntu16.04:

## Installation

```bash
# Install pip
Curl -s https://bootstrap.pypa.io/get-pip.py | python3

# Install the latest version of docker
Curl -s https://get.docker.com/ | sh

#Start docker service
Service docker start

#Install compose
Pip install docker-compose
```

Other operating system installations of docker and docker-compose may be slightly different, please read the Docker documentation for installation.

## Usage

```bash
# Pulling items
Git clone https://github.com/vulhub/vulhub.git
Cd vulhub

# Enter a directory of a vulnerability/environment
Cd flask/ssti

#自动化编译 environment
Docker-compose build

#Start the entire environment
Docker-compose up -d
```

There is a corresponding documentation in each environment directory, please read this file for vulnerability/environment testing.

After the test is complete, delete the entire environment

```
Docker-compose down
```

Each vulnerability environment of this project is accompanied by documentation. It is recommended that you purchase a 1G memory vps build vulnerability test environment. The `your-ip` mentioned in the document refers to the IP address of your vps. If you are using a virtual machine to build a test environment. , refers to your virtual machine IP, not the IP inside the docker container, please don't confuse.

**All environments in this project are for testing purposes only and should not be used as a production environment! **

## Notice

Precautions:

1. To prevent permission errors, it is best to use the root user to execute the docker and docker-compose commands.
2. The docker partial image does not support running on machines such as ARM.

## Contribution

This project relies on docker. Any exceptions that occur during compilation and running are thrown by docker and related programs. Please find the cause of the error yourself. If it is determined that the Dockerfile is written incorrectly (or the code is wrong in vulhub), then submit the issue. For more instructions, please [this document] (https://github.com/phith0n/vulhub/wiki/%E7%BC%96%E8%AF%91%E5%A4%B1%E8%B4%A5%E7 %9A%84%E5%8E%9F%E5%9B%A0), I hope I can help you.

Thanks list: [Contributors List] (contributors.md)

## License

Vulhub is released under the [GPL-3.0 license](LICENSE).