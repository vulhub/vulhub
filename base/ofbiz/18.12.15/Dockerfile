FROM openjdk:8-jdk

LABEL maintainer="phith0n <root@leavesongs.com>"

ARG VERSION=18.12.15
RUN set -ex \
    && cd /usr/src \
    && wget -q http://archive.apache.org/dist/ofbiz/apache-ofbiz-${VERSION}.zip \
    && unzip apache-ofbiz-${VERSION}.zip \
    && mv apache-ofbiz-${VERSION} apache-ofbiz \
    && rm -rf /usr/src/apache-ofbiz-${VERSION}.zip

RUN set -ex \
    && cd /usr/src/apache-ofbiz \
    && sed -i '/throw new RequestHandlerException.*not accepted to prevent host header injection/,/You need to set host-headers-allowed property in security.properties file.")/s/^/\/\/ /' framework/webapp/src/main/java/org/apache/ofbiz/webapp/control/RequestHandler.java \
    && ./gradle/init-gradle-wrapper.sh \
    && ./gradlew loadAll
    # && ./gradlew "ofbiz --load-data readers=seed,seed-initial,ext"

WORKDIR /usr/src/apache-ofbiz
EXPOSE 8443
CMD [ "java", "-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005", "-jar", "./build/libs/ofbiz.jar" ]
