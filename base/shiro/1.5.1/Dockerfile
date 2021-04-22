FROM maven:3-jdk-8 AS builder

LABEL MAINTAINER="phithon <root@leavesongs.com>"

COPY ./code/ /usr/src/

WORKDIR /usr/src

RUN cd /usr/src; \
    mvn -U clean package -Dmaven.test.skip=true

FROM openjdk:8-jre

LABEL MAINTAINER="phithon <root@leavesongs.com>"

COPY --from=builder /usr/src/target/shirodemo-1.0-SNAPSHOT.jar /shirodemo-1.0-SNAPSHOT.jar

EXPOSE 8080

CMD ["java", "-jar", "/shirodemo-1.0-SNAPSHOT.jar"]