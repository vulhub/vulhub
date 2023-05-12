FROM maven:3-jdk-7 AS builder

LABEL MAINTAINER="phithon <root@leavesongs.com>"

COPY ./ /usr/src/

WORKDIR /usr/src

RUN cd /usr/src; \
    mvn -U clean package -Dmaven.test.skip=true

FROM vulhub/java:7u21-jdk

LABEL MAINTAINER="b1ngz <blinking.yan@gmail.com>"

COPY --from=builder /usr/src/target/jackson.jar /jackson.jar

EXPOSE 8080

CMD ["java", "-jar", "/jackson.jar"]