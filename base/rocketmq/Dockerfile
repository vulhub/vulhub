FROM ubuntu:22.04

LABEL MAINTAINER="4ra1n"

COPY ./rocketmq-all-5.1.0-bin-release.zip /rocketmq.zip
COPY ./runbroker.sh /runbroker.sh

RUN sed -i s@/archive.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list; \
	sed -i s@/security.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list; \
	apt clean; \
	apt update; \
	apt install -y unzip; \
	apt install -y openjdk-8-jdk;

RUN unzip /rocketmq.zip; \
	mv /runbroker.sh /rocketmq-all-5.1.0-bin-release/bin/runbroker.sh; \
        chmod 777 /rocketmq-all-5.1.0-bin-release/bin/runbroker.sh;

EXPOSE 10911

CMD sh -c /rocketmq-all-5.1.0-bin-release/bin/mqbroker -n localhost:9876 --enable-proxy
