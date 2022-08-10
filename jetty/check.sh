printf 'GET / HTTP/1.1\r\n'\
'Host: localhost\r\n'\
'Connection: close\r\n'\
'\r\n'\
| nc 127.0.0.1 9014