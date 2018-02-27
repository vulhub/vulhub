FROM openjdk:8-jre

LABEL maintainer="phithon <root@leavesongs.com>"

RUN apt-get update \
    && apt-get install -y wget unzip \
    && cd /usr/src \
    && wget https://archive.apache.org/dist/jmeter/binaries/apache-jmeter-3.3.zip \
    && unzip apache-jmeter-3.3.zip \
    && apt-get purge --autoremove -y wget unzip \
    && rm -rf /usr/src/apache-jmeter-3.3.zip /var/lib/apt/lists/*

EXPOSE 1099

CMD ["/usr/src/apache-jmeter-3.3/bin/jmeter-server"]