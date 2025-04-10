FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y openjdk-8-jdk && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*
    
ENV JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
ENV PATH=$PATH:$JAVA_HOME/bin
RUN mkdir -p /opt/tomcat
COPY ./tomcat /opt/tomcat
WORKDIR /opt/tomcat

RUN chmod +x /opt/tomcat/bin/*.sh

EXPOSE 8080

CMD ["/opt/tomcat/bin/catalina.sh", "run"]