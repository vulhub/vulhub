FROM maven:3-jdk-8

MAINTAINER phithon <root@leavesongs.com>

COPY ./ /usr/src/

WORKDIR /usr/src

RUN cd /usr/src; \
    mvn clean install

CMD ["java", "-Djava.security.egd=file:/dev/./urandom", "-jar", "./target/spring-rest-data-demo-2.0.0.BUILD.jar"]
