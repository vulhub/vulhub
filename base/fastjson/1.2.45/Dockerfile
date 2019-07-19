FROM maven:3.6.1-jdk-8

LABEL maintainer="phithon <root@leavesongs.com>"

COPY ./src pom.xml /usr/src/
WORKDIR /usr/src

RUN set -ex \
    && mvn compile

CMD [ "mvn", "-Dserver.address=0.0.0.0", "-Dserver.port=8090", "spring-boot:run" ]