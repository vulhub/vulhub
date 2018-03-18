FROM vulhub/java:8u162-jdk AS build-env

RUN mkdir /opt/maven \
    && wget -qO- http://archive.apache.org/dist/maven/binaries/apache-maven-3.2.2-bin.tar.gz | tar -xz -C /opt/maven --strip-components=1

COPY . /usr/src/

RUN set -ex \
    && cd /usr/src \
    && /opt/maven/bin/mvn package assembly:single

FROM openjdk:8-jre-slim

LABEL maintainer="phithon <root@leavesongs.com>"

COPY --from=build-env /usr/src/target/log4jrce-1.0-SNAPSHOT-all.jar /log4jrce-1.0-SNAPSHOT-all.jar

CMD ["java", "-jar", "/log4jrce-1.0-SNAPSHOT-all.jar"]