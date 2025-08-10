FROM python:3.11-slim

LABEL maintainer="hx294 <hx294@qq.com>"

WORKDIR /app

COPY requirements.txt aiohttpServer.py /app/

RUN mkdir -p /app/static \
    && pip install --no-cache-dir -r requirements.txt

EXPOSE 8080

CMD ["python", "aiohttpServer.py"]