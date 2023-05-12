FROM maven:3-jdk-8 AS builder

LABEL maintainer="phithon <root@leavesongs.com>"

COPY ./ /usr/src/

WORKDIR /usr/src

RUN cd /usr/src; \
    mvn -U clean package -Dmaven.test.skip=true

FROM openjdk:8u252-jre

LABEL maintainer="phithon <root@leavesongs.com>"

COPY --from=builder /usr/src/target/h2-console-demo-0.0.1-SNAPSHOT.jar /h2-console-demo-0.0.1-SNAPSHOT.jar

EXPOSE 8080

CMD ["java", "-jar", "/h2-console-demo-0.0.1-SNAPSHOT.jar"]