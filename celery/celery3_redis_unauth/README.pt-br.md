# Celery <4.0 Redis unauthorized access + Pickle deserialization exploit

O Celery é um sistema distribuído simples, flexível e confiável que processa um grande número de mensagens enquanto fornece as ferramentas necessárias para operar e manter esse sistema. É uma fila de tarefas focada no processamento em tempo real e também suporta agendamento de tarefas.

Na versão Celery <4.0, Pickle é usado por padrão para mensagens de tarefas serializadas. Quando o serviço de fila (por exemplo, Redis/RabbitMQ/RocketMQ ） usado tem um problema de acesso não autorizado, a vulnerabilidade de desserialização do Pickle pode ser usada para executar código arbitrário.

## Ambiente de vulnerabilidade

Execute o seguinte comando para iniciar o Celery 3.1.23 + Redis:

```bash
docker-compose up -d
```

## Explorando

O script de exploração `exploit.py` só suporta o uso em python3

```bash
pip install redis
python exploit.py [Host IP]
```

Ver resultados:

```
docker-compose logs celery
```

Você pode ver o seguinte erro de mensagem de tarefa:

![](a.png)

```bash
docker-compose exec celery ls -l /tmp
```

Você pode ver que o arquivo `celery_success` foi criado com sucesso

![](b.png)

## Referências

https://docs.celeryproject.org/en/stable/userguide/configuration.html

https://www.bookstack.cn/read/celery-3.1.7-zh/8d5b10e3439dbe1f.md#dhfmrk

https://docs.celeryproject.org/en/stable/userguide/calling.html#serializers

https://www.jianshu.com/p/52552c075bc0

https://www.runoob.com/w3cnote/python-redis-intro.html

https://blog.csdn.net/SKI_12/article/details/85015803
