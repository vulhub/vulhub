FROM python:2.7.14-alpine3.6
COPY repositories /etc/apk
RUN apk update
RUN apk add gcc
RUN apk add build-base
RUN mkdir /root/.pip
COPY pip.conf /root/.pip
RUN pip install flask
RUN pip install pycrypto
COPY app /root/app
CMD ["python", "/root/app/app.py"]
