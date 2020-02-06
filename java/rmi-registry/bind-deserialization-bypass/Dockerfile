FROM openjdk:8u121-jre

WORKDIR /root
ENV RMIIP="127.0.0.1"
COPY src/RMITrain-all.jar /root/
COPY src/client.policy /root/
EXPOSE 1099

CMD ["bash", "-c", "java -cp RMITrain-all.jar -Djdk.xml.enableTemplatesImplDeserialization=true -Djava.rmi.server.hostname=${RMIIP} -Djava.security.manager -Djava.security.policy=/root/client.policy train.rmi.Server"]