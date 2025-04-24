FROM python:3.9-slim

RUN mkdir /app && pip install --no-cache-dir "gradio==4.10.0"

WORKDIR /app

EXPOSE 7860

ENV GRADIO_SERVER_NAME="0.0.0.0"

CMD ["python", "app.py"]