<!-- markdownlint-disable first-line-heading -->
<p align="center">
  <img src=".github/assets/logo.svg" alt="Vulhub" height="300" />
  <p align="center">
    <a href="https://github.com/vulhub/vulhub/blob/master/LICENSE">
      <img src="https://img.shields.io/github/license/vulhub/vulhub.svg" alt="GitHub">
    </a>
    <a href="https://www.wangan.com/vulhub">
      <img src="https://img.shields.io/badge/Official-Community-blue.svg" alt="Official Community">
    </a>
    <a href="https://discord.gg/GhMB3Z">
      <img src="https://img.shields.io/discord/485505185167179778.svg" alt="Chat on Discord">
    </a>
    <a href="https://www.patreon.com/phith0n">
      <img src="https://img.shields.io/badge/sponsor-patreon-73d6a1.svg" alt="Backers and sponors on Patreon">
    </a>
    <a href="https://opencollective.com/vulhub#backer">
      <img src="https://img.shields.io/badge/backer-opencollective-f89a76.svg" alt="Backers and sponors on Opencollective">
    </a>
  </p>
</p>

Vulhub é uma coleção de código aberto de ambientes docker vulneráveis pré-criados. Nenhum conhecimento pré-existente do docker é necessário, basta executar dois comandos simples e você terá um ambiente vulnerável.

[中文版本(Chinese version)](README.zh-cn.md)
[PT-BR(Portuguese version)](./README.pt-br.md)

## Instalação

Instale o docker/docker-compose no Ubuntu 20.04:

```bash
# Instalar pip
curl -s https://bootstrap.pypa.io/get-pip.py | python3

# Instale a janela de encaixe da versão mais recente
curl -s https://get.docker.com/ | sh

# Executa o serviço docker
systemctl start docker

# Instala a composição do docker
pip install docker-compose
```

As etapas de instalação do docker e do docker-compose para outros sistemas operacionais podem ser ligeiramente diferentes, consulte a [documentação do docker](https://docs.docker.com/) para obter detalhes.

## Uso

```bash
# Baixar projeto
wget https://github.com/vulhub/vulhub/archive/master.zip -O vulhub-master.zip
unzip vulhub-master.zip
cd vulhub-master

# Entre no diretório de vulnerabilidade/ambiente
cd flask/ssti

# Compilar ambiente
docker-compose build

# Executar ambiente
docker-compose up -d
```

Existe um documento **README** em cada diretório de ambiente, por favor leia este arquivo para teste e uso de vulnerabilidade/ambiente.

Após o teste, exclua o ambiente com o seguinte comando.

```
docker-compose down -v
```

Recomenda-se usar um VPS de pelo menos 1 GB de memória para criar um ambiente de vulnerabilidade. O `your-ip` mencionado na documentação refere-se ao endereço IP do seu VPS. Se você estiver usando uma máquina virtual, ele se refere ao IP da máquina virtual, não ao IP dentro do contêiner docker.

**Todos os ambientes neste projeto são apenas para fins de teste e não devem ser usados como ambiente de produção!**

## Aviso prévio

1. Para evitar erros de permissão, é melhor usar o usuário root para executar os comandos docker e docker-compose.
2. Algumas imagens docker não suportam execução em máquinas ARM.

## Contribuição

Este projeto depende do docker. Portanto, qualquer erro durante a compilação e execução é gerado pelo docker e programas relacionados. Por favor, encontre a causa do erro por si mesmo primeiro. Se for determinado que o dockerfile está escrito incorretamente (ou o código está errado no vulhub), envie o problema. Mais detalhes por favor 👉[Motivos comuns para falha de compilação](https://github.com/phith0n/vulhub/wiki/%E7%BC%96%E8%AF%91%E5%A4%B1%E8%B4%A5%E7%9A%84%E5%8E%9F%E5%9B%A0), espero que possa ajudá-lo.

Para mais perguntas, entre em contato:

- [Comunidade chinesa](https://www.wangan.com/vulhub)
- [Discord](https://discord.gg/GhMB3Z)
- [Twitter](https://twitter.com/vulhub)

Obrigado aos seguintes colaboradores:

[![](https://opencollective.com/vulhub/contributors.svg?width=890&button=false)](https://github.com/vulhub/vulhub/graphs/contributors)

Mais contribuidores：[Lista de contribuidores](contributors.md)

## Parceiros

Nossos Parceiros e Usuários:

<p>
  <a href="https://www.wangan.com/vulhub" target="_blank"><img src="https://vulhub.org/img/sponsor/wangan.png" width="200"></a>
  <a href="https://www.cvebase.com" target="_blank"><img src="https://vulhub.org/img/sponsor/cvebase.png" width="200"></a>
  <a href="https://www.huoxian.cn" target="_blank"><img src="https://vulhub.org/img/sponsor/huoxian.png" width="200"></a>
  <a href="https://www.chaitin.cn" target="_blank"><img src="https://vulhub.org/img/sponsor/chaitin.png" width="200"></a>
  <a href="https://xianzhi.aliyun.com/" target="_blank"><img src="https://vulhub.org/img/sponsor/aliyun.svg" width="200"></a>
</p>

Patrocine vulhub no patreon 🙏

<a href="https://www.patreon.com/bePatron?u=12677520"><img src="https://vulhub.org/img/sponsor/patreon.png" width="150"></a>

Patrocine vulhub no opencollective 🙏

<p>
  <a href="https://opencollective.com/vulhub#backer"><img src="https://opencollective.com/vulhub/backers.svg?width=138"></a>
  <a href="https://opencollective.com/vulhub#sponsor"><img src="https://opencollective.com/vulhub/sponsors.svg?width=138"></a>
</p>

Mais [doações](http://vulhub.org/#/docs/donate/).

## Licença

Vulhub é licenciado sob a Licença MIT. Consulte [LICENSE](LICENSE) para obter o texto completo da licença.
