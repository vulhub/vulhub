FROM maven:3-jdk-8 AS builder

LABEL MAINTAINER="phithon <root@leavesongs.com>"

COPY ./src/code/ /usr/src/

WORKDIR /usr/src

RUN cd /usr/src; \
    mvn -U clean package -Dmaven.test.skip=true --settings settings.xml

FROM openjdk:8u131-jre

WORKDIR /root
ENV RMIIP="127.0.0.1"
COPY --from=builder /usr/src/target/train-1.0-SNAPSHOT-all.jar /root/train-1.0-SNAPSHOT-all.jar
COPY src/client.policy /root/
EXPOSE 1099

CMD ["bash", "-c", "java -cp train-1.0-SNAPSHOT-all.jar -Djdk.xml.enableTemplatesImplDeserialization=true -Djava.rmi.server.hostname=${RMIIP} -Djava.security.manager -Djava.security.policy=/root/client.policy train.rmi.Server"]
