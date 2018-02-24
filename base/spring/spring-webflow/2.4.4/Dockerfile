FROM vulhub/maven:3.5.2-jdk-8 AS build-env

RUN set -ex \
    && apt-get update \
    && apt-get install -y wget unzip \
    && cd /usr/src/ \
    && wget https://github.com/spring-projects/spring-webflow-samples/archive/f2312608cb924666d5d068e8016c1727b006d372.zip -O spring-webflow-samples.zip \
    && unzip spring-webflow-samples.zip \
    && cd spring-webflow-samples-f2312608cb924666d5d068e8016c1727b006d372/booking-mvc \
    && mvn clean install

FROM vulhub/tomcat:8.0

MAINTAINER phithon <root@leavesongs.com>

COPY --from=build-env /usr/src/spring-webflow-samples-master/booking-mvc/target/booking-mvc.war /usr/local/tomcat/webapps/ROOT.war