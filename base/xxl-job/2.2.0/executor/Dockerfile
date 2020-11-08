FROM maven:3.6.3-jdk-8 AS builder

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && cd /usr/src \
    && git clone https://github.com/xuxueli/xxl-job . \
    && git checkout v2.2.0 \
    && cd xxl-job-executor-samples/xxl-job-executor-sample-springboot \
    && sed -i 's|xxl\.job\.admin\.addresses=.*|xxl.job.admin.addresses=http://admin:8080/xxl-job-admin|g' src/main/resources/application.properties \
    && sed -i 's|xxl\.job\.executor\.logpath=.*|xxl.job.executor.logpath=/var/log/xxl-job|g' src/main/resources/application.properties \
    && mvn clean package -DskipTests

FROM openjdk:8u272-jre

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends wait-for-it \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /usr/src/xxl-job-executor-samples/xxl-job-executor-sample-springboot/target/xxl-job-executor-sample-springboot-2.2.0.jar /usr/src/xxl-job-executor-sample-springboot-2.2.0.jar
COPY docker-entrypoint.sh /docker-entrypoint.sh

WORKDIR /usr/src
ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["java", "-jar", "/usr/src/xxl-job-executor-sample-springboot-2.2.0.jar"]
