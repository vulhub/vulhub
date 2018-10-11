FROM vulhub/java:7u21-jdk

LABEL MAINTAINER="b1ngz <blinking.yan@gmail.com>"

COPY target/jackson.jar /root/jackson.jar

EXPOSE 8080

CMD ["java", "-jar", "/root/jackson.jar"]