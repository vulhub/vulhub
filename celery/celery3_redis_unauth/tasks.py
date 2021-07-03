from celery import Celery
app = Celery('tasks', broker='redis://redis/0',backend='redis://redis/1')
app.conf['CELERY_ACCEPT_CONTENT'] = ['pickle', 'json', 'msgpack', 'yaml']
@app.task
def add(x, y):
    return x + y
