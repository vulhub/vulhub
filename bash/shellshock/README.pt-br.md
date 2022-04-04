# Shellshock Remote Command Injection (CVE-2014-6271)

Construa e execute o ambiente vulnerável:

```
docker-compose build
docker-compose up -d
```

Quando você visita `http://your-ip/` você deve ver dois arquivos:

- safe.cgi
- victim.cgi

safe.cgi gerado pela última versão do bash, e vitima.cgi é a página gerada pelo bash4.3 que é vulnerável ao shellshock.

Podemos enviar include nosso payload na string user-agent ao visitar vítima.cgi e o comando é executado com sucesso:

![](1.png)

A mesma solicitação enviada para safe.cgi não é afetada:

![](2.png)
