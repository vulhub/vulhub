FROM maven:3.9-eclipse-temurin-17

LABEL maintainer="phithon <root@leavesongs.com>"

COPY pom.xml /usr/src/pom.xml

WORKDIR /usr/src

RUN mvn -U dependency:resolve dependency:resolve-plugins

COPY src /usr/src/src

EXPOSE 8080 5005

CMD ["mvn", "spring-boot:run", "-Dspring-boot.run.jvmArguments=-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=*:5005"]
