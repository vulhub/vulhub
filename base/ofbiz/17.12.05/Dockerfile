FROM openjdk:8-jdk

LABEL maintainer="phith0n <root@leavesongs.com>"

ARG VERSION=17.12.05
RUN set -ex \
    && cd /usr/src \
    && wget http://archive.apache.org/dist/ofbiz/apache-ofbiz-${VERSION}.zip \
    && unzip apache-ofbiz-${VERSION}.zip \
    && mv apache-ofbiz-${VERSION} apache-ofbiz \
    && rm -rf /usr/src/apache-ofbiz-${VERSION}.zip

RUN set -ex \
    && cd /usr/src/apache-ofbiz \
    && sed -i 's/at\.bxm\.gradleplugins:gradle-svntools-plugin:latest\.release/at.bxm.gradleplugins:gradle-svntools-plugin:2.2.1/g' build.gradle \
    && ./gradle/init-gradle-wrapper.sh \
    && sed -i 's|distributions-snapshots/gradle-3.2.1-20161121174103+0000|distributions/gradle-3.2.1|i' ./gradle/wrapper/gradle-wrapper.properties \
    # && ./gradlew "ofbiz --load-data readers=seed,seed-initial,ext"
    && ./gradlew loadAll

WORKDIR /usr/src/apache-ofbiz
EXPOSE 8443
CMD [ "java", "-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005", "-jar", "./build/libs/ofbiz.jar" ]