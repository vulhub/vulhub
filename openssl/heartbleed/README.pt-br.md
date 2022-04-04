# OpenSSL Heartbleed Vulnerability (CVE-2014-0160)

O Heartbleed Bug é uma vulnerabilidade séria na popular biblioteca de software criptográfico OpenSSL. Esta fraqueza permite roubar a informação protegida, em condições normais, pela encriptação SSL/TLS utilizada para proteger a Internet. SSL/TLS fornece segurança de comunicação e privacidade na Internet para aplicativos como web, e-mail, mensagens instantâneas (IM) e algumas redes privadas virtuais (VPNs).

O bug Heartbleed permite que qualquer pessoa na Internet leia a memória dos sistemas protegidos pelas versões vulneráveis do software OpenSSL. Isso compromete as chaves secretas usadas para identificar os provedores de serviços e criptografar o tráfego, os nomes e senhas dos usuários e o conteúdo real. Isso permite que os invasores escutem as comunicações, roubem dados diretamente dos serviços e usuários e personifiquem serviços e usuários.

Referências:

- https://heartbleed.com/
- https://filippo.io/Heartbleed

## Configurar

Construir e executar:

```
docker-compose up -d
```

## Prova de Conceito

Visite `https://filippo.io/Heartbleed` para conferir o resultado:

![](1.png)

Execute [ssltest.py](ssltest.py) com Python para obter dados confidenciais (Cookie)：

![](2.png)
