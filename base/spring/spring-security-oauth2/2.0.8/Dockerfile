FROM maven:3-jdk-8

COPY src/ /usr/src/
WORKDIR /usr/src

RUN cd /usr/src; \
    mvn clean install

CMD ["java", "-Djava.security.egd=file:/dev/./urandom", "-jar", "./target/demo-0.0.1-SNAPSHOT.jar"]
