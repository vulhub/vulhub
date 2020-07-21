FROM vulhub/java:7u21-jdk

LABEL maintainer="phithon <root@leavesongs.com>"

COPY . /usr/src/

RUN set -ex \
    && cd /usr/src/ \
    && mvn clean compile jetty:help

EXPOSE 8080
WORKDIR /usr/src
CMD ["mvn", "jetty:run"]
