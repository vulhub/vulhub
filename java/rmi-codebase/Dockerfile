FROM openjdk:8u222-jdk

LABEL maintainer="phithon <root@leavesongs.com>"

ENV RMIIP="127.0.0.1"
COPY src/ /usr/src/
WORKDIR /usr/src

RUN set -ex \
    && javac *.java

CMD ["bash", "-c", "java -Djava.rmi.server.hostname=${RMIIP} -Djava.rmi.server.useCodebaseOnly=false -Djava.security.policy=client.policy RemoteRMIServer"]
