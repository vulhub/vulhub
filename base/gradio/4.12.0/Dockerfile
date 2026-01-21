FROM python:3.9-slim

RUN mkdir /app && pip install --no-cache-dir "gradio==4.12.0" "fastapi==0.112.4" "pydantic==2.8.2"

WORKDIR /app

COPY app.py /app/app.py

EXPOSE 7860

ENV GRADIO_SERVER_NAME="0.0.0.0"

CMD ["python", "app.py"]
