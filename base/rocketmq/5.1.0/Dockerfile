FROM openjdk:8-jdk

LABEL MAINTAINER="4ra1n"

COPY ./rocketmq-all-5.1.0-bin-release.zip /rocketmq.zip
COPY ./runbroker.sh /runbroker.sh

RUN unzip /rocketmq.zip; \
        mv /runbroker.sh /rocketmq-all-5.1.0-bin-release/bin/runbroker.sh; \
        chmod 777 /rocketmq-all-5.1.0-bin-release/bin/runbroker.sh;

EXPOSE 10911

CMD sh -c /rocketmq-all-5.1.0-bin-release/bin/mqbroker -n localhost:9876 --enable-proxy