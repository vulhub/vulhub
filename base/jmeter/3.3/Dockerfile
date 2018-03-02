FROM vulhub/java:8u20-jdk

LABEL maintainer="phithon <root@leavesongs.com>"

ENV JMETER_VERSION 3.3

RUN apt-get update \
    && apt-get install -y wget unzip \
    && cd /usr/src \
    && wget https://archive.apache.org/dist/jmeter/binaries/apache-jmeter-${JMETER_VERSION}.zip \
    && unzip apache-jmeter-${JMETER_VERSION}.zip \
    && cd apache-jmeter-${JMETER_VERSION}/bin \
    && chmod +x jmeter jmeter-server \
    && apt-get purge --autoremove -y wget unzip \
    && rm -rf /usr/src/apache-jmeter-${JMETER_VERSION}.zip /var/lib/apt/lists/*

EXPOSE 1099

CMD /usr/src/apache-jmeter-${JMETER_VERSION}/bin/jmeter-server