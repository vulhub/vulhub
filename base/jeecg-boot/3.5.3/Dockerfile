FROM maven:3.8.6-jdk-8 AS builder

LABEL maintainer="ReaJason <reajason1225@gmail.com>"

RUN set -ex \
    && cd /usr/src \
    && git clone https://github.com/jeecgboot/jeecg-boot.git \
    && cd jeecg-boot \
    && git checkout v3.5.3 \
    && sed -i 's/host: 127.0.0.1/host: jeecg-boot-redis/' jeecg-module-system/jeecg-system-start/src/main/resources/application-dev.yml \
    && sed -i 's|mysql://127\.0\.0\.1:3306|mysql://jeecg-boot-mysql:3306|g' jeecg-module-system/jeecg-system-start/src/main/resources/application-dev.yml \
    && mvn clean package -DskipTests

FROM openjdk:8u342-jre

LABEL maintainer="ReaJason <reajason1225@gmail.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends wait-for-it default-mysql-client \
    && rm -rf /var/lib/apt/lists/*


COPY --from=builder /usr/src/jeecg-boot/jeecg-module-system/jeecg-system-start/target/jeecg-system-start-3.5.3.jar /usr/src/jeecg-system-start-3.5.3.jar
COPY docker-entrypoint.sh /docker-entrypoint.sh
COPY --from=builder /usr/src/jeecg-boot/db/jeecgboot-mysql-5.7.sql /usr/src/jeecgboot-mysql-5.7.sql

EXPOSE 8080

WORKDIR /usr/src
ENTRYPOINT ["/docker-entrypoint.sh"]
CMD java -Djava.security.egd=file:/dev/./urandom $JAVA_OPTS -jar /usr/src/jeecg-system-start-3.5.3.jar