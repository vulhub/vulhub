FROM openjdk:8-jdk

LABEL maintainer="phith0n <root@leavesongs.com>"

RUN set -ex \
    && cd /usr/src \
    && wget http://archive.apache.org/dist/ofbiz/apache-ofbiz-17.12.01.zip \
    && unzip apache-ofbiz-17.12.01.zip \
    && rm -rf /usr/src/apache-ofbiz-17.12.01.zip

RUN set -ex \
    && cd /usr/src/apache-ofbiz-17.12.01 \
    && sed -i 's/at\.bxm\.gradleplugins:gradle-svntools-plugin:latest\.release/at.bxm.gradleplugins:gradle-svntools-plugin:2.2.1/g' build.gradle \
    && ./gradle/init-gradle-wrapper.sh \
    && ./gradlew "ofbiz --load-data readers=seed,seed-initial,ext"

WORKDIR /usr/src/apache-ofbiz-17.12.01
EXPOSE 8443
CMD [ "./gradlew", "ofbiz" ]
