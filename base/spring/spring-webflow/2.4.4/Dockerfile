FROM maven:3-jdk-8 AS build-env

RUN set -ex \
    && apt-get update \
    && apt-get install -y wget unzip \
    && cd /usr/src/ \
    && wget https://github.com/spring-projects/spring-webflow-samples/archive/f2312608cb924666d5d068e8016c1727b006d372.zip -O spring-webflow-samples.zip \
    && unzip spring-webflow-samples.zip \
    && cd spring-webflow-samples-f2312608cb924666d5d068e8016c1727b006d372/booking-mvc \
    && sed -i 's/factoryCreator\.setUseSpringBeanBinding.*;//g' src/main/java/org/springframework/webflow/samples/booking/config/WebFlowConfig.java \
    && mvn clean install

FROM vulhub/tomcat:8.0

LABEL maintainer="phithon <root@leavesongs.com>"

RUN rm -rf /usr/local/tomcat/webapps/* \
    && sed -i 's/securerandom.source=file:\/dev\/urandom/securerandom.source=file:\/dev\/.\/urandom/g' $JAVA_HOME/lib/security/java.security

COPY --from=build-env /usr/src/spring-webflow-samples-f2312608cb924666d5d068e8016c1727b006d372/booking-mvc/target/booking-mvc.war /usr/local/tomcat/webapps/ROOT.war