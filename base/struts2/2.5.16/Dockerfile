FROM maven:3-jdk-8

LABEL maintainer="litch1<litch1chk@gmail.com>"

COPY ./ /usr/src/
WORKDIR /usr/src

RUN set -ex \
    && mvn compile jetty:help

EXPOSE 8080
CMD ["mvn", "jetty:run"]
