# Vulhub - Some Docker-Compose files for vulnerabilities environment

Vulhub是一个面向大众的开源漏洞靶场，无需docker知识，简单执行两条命令即可编译、运行一个完整的漏洞靶场镜像。

安装docker-compose:

```bash
# 安装pip
curl -s https://bootstrap.pypa.io/get-pip.py | python3

# 安装docker
apt-get install docker.io

# 安装compose
pip install docker-compose 
```

启动

```bash
# 拉取项目
git clone git@github.com:phith0n/vulhub.git
cd vulhub

# 进入某一个漏洞/环境的目录
cd nginx_php5_mysql

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

**本项目中所有环境仅用于测试，不可作为生成环境使用！**