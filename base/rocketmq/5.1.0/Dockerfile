FROM eclipse-temurin:8-jre

LABEL MAINTAINER="4ra1n"

RUN set -ex; \
        wget -qO/rocketmq.zip https://archive.apache.org/dist/rocketmq/5.1.0/rocketmq-all-5.1.0-bin-release.zip; \
        unzip /rocketmq.zip; \
        rm -f /rocketmq.zip; \
        sed -i 's/Xms8g/Xms1g/g' /rocketmq-all-5.1.0-bin-release/bin/runbroker.sh; \
        sed -i 's/Xmx8g/Xmx1g/g' /rocketmq-all-5.1.0-bin-release/bin/runbroker.sh

WORKDIR /rocketmq-all-5.1.0-bin-release
ENV JAVA_OPTS="-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005"
EXPOSE 10911 5005

CMD sh -c /rocketmq-all-5.1.0-bin-release/bin/mqbroker -n localhost:9876 --enable-proxy
