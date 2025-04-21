FROM vulhub/java:7u21-jdk

LABEL maintainer="phithon <root@leavesongs.com>"
ENV PATH=${PATH}:/opt/maven/bin

RUN set -ex \
    && mkdir /opt/maven \
    && wget -qO- https://archive.apache.org/dist/maven/maven-3/3.5.0/binaries/apache-maven-3.5.0-bin.tar.gz | tar xz --strip-components 1 -C /opt/maven

COPY . /usr/src/
RUN set -ex \
    && cd /usr/src/ \
    && mvn -Dhttps.protocols=TLSv1.2 compile jetty:effective-web-xml

EXPOSE 8080
WORKDIR /usr/src
CMD ["mvn", "jetty:run"]