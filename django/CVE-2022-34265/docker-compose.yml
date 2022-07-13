version: '2'
services:
  web:
    image: vulhub/django:4.0.5
    ports:
    - "8000:8000"
    depends_on:
    - db
    volumes:
    - ./web:/usr/src
    - ./docker-entrypoint.sh:/docker-entrypoint.sh
    entrypoint:
    - bash
    - /docker-entrypoint.sh
    command:
    - python
    - app.py
    - runserver
    - 0.0.0.0:8000
  db:
   image: postgres:13-alpine
   environment:
    - POSTGRES_PASSWORD=postgres
    - POSTGRES_DB=CVE_2022_34265
