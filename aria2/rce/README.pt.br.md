# Aria2 Arbitrary File Write Vulnerability

Aria2 é uma ferramenta de download leve, multiprotocolo e multifonte (suporta HTTP/HTTPS, FTP, BitTorrent, Metalink) com interfaces XML-RPC e JSON-RPC integradas.

Podemos usar a interface RPC para operar aria2 e baixar arquivos para qualquer diretório, causando uma vulnerabilidade arbitrária de gravação de arquivos.

Artigo de referência:

- https://paper.seebug.org/120/

## Ambiente vulnerável

Inicie o ambiente vulnerável:

```
docker-compose up -d
```

6800 é a porta padrão do serviço rpc da aria2. Após o ambiente ser iniciado, acesse `http://your-ip:6800/`, e o serviço deve retornar uma página 404.

## Explorando

Como a comunicação rpc requer json ou xml, não é conveniente, portanto, podemos usar uma interface de usuário de terceiros para nos comunicar com o destino, como http://binux.github.io/yaaw/demo/

Abra o yaaw, clique no botão configurar e preencha o nome do domínio alvo executando aria2: `http://your-ip:6800/jsonrpc`:

![](1.png)

Em seguida, clique em "Adicionar +" para adicionar uma nova tarefa de download. Preencha o campo "Dir" com o diretório para o qual deseja que seu arquivo seja baixado e preencha o campo "Nome do arquivo" com o nome do arquivo desejado. Por exemplo, vamos baixar um shell reverso escrevendo uma tarefa crond:

![](2.png)

Neste momento, arai2 baixará o arquivo malicioso (a URL que você especificou) para o diretório /etc/cron.d/, com o nome de arquivo "shell". No debian, todos os arquivos no diretório /etc/cron.d serão lidos como um arquivo de configuração de tarefa agendada (como crontab). Uma vez escrito, podemos esperar até um minuto antes que o script shell reverso seja executado:

![](3.png)

> Se o shell reverso não for bem-sucedido, observe o formato do arquivo crontab, e a nova linha deve ser `\n`, e uma nova linha é necessária no final do arquivo.

Claro, também podemos tentar escrever outros arquivos, para mais maneiras de explorar essa vulnerabilidade, consulte [este artigo][1]

[1]: https://paper.seebug.org/120/
