FROM eclipse-temurin:8-jre

LABEL MAINTAINER="4ra1n"

RUN set -ex; \
        apt-get update; \
        apt-get install --no-install-recommends -y ca-certificates wget unzip; \
        wget -O /rocketmq.zip https://archive.apache.org/dist/rocketmq/5.1.0/rocketmq-all-5.1.0-bin-release.zip; \
        unzip /rocketmq.zip; \
        rm -rf /rocketmq.zip /var/lib/apt/lists/*;

WORKDIR /rocketmq-all-5.1.0-bin-release
COPY docker-entrypoint.sh /docker-entrypoint.sh
RUN set -ex; \
        sed -i 's/Xms8g/Xms1g/g' ./bin/runbroker.sh; \
        sed -i 's/Xmx8g/Xmx1g/g' ./bin/runbroker.sh; \
        sed -i 's/Xmn4g/Xmn512m/g' ./bin/runbroker.sh; \
        sed -i 's/Xms4g/Xms1g/g' ./bin/runserver.sh; \
        sed -i 's/Xmx4g/Xmx1g/g' ./bin/runserver.sh; \
        sed -i 's/Xmn2g/Xmn512m/g' ./bin/runserver.sh; \
        sed -i 's/which javac/which java/g' ./bin/runserver.sh; \
        sed -i 's/which javac/which java/g' ./bin/runbroker.sh; \
        sed -i 's/which javac/which java/g' ./bin/tools.sh; \
        chmod +x /docker-entrypoint.sh

ENV JAVA_OPT="-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005"
EXPOSE 9876 10911 5005 8080 8081

ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["mqbroker", "-n", "localhost:9876", "--enable-proxy"]
