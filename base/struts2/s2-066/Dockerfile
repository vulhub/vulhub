FROM maven:3.9.9-eclipse-temurin-17 AS builder

COPY . /usr/src

RUN cd /usr/src && mvn clean package -DskipTests

FROM tomcat:9.0.99-jdk17-temurin

COPY --from=builder /usr/src/target/struts2-s2-066.war /usr/local/tomcat/webapps/ROOT.war
