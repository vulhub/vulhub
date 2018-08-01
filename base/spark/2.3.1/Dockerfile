FROM openjdk:8-jre

RUN wget -qO- http://www-us.apache.org/dist/spark/spark-2.3.1/spark-2.3.1-bin-hadoop2.7.tgz | tar zx -C /usr/local/spark --strip-components=1

WORKDIR /usr/local/spark
EXPOSE 8080 7077 6066

ENTRYPOINT ./sbin/start-master.sh && tail -f logs/*.out