FROM openjdk:8-jre

ARG basedir=/usr/local/spark

RUN mkdir $basedir \
    && wget -qO- http://www-us.apache.org/dist/spark/spark-2.3.1/spark-2.3.1-bin-hadoop2.7.tgz | tar zx -C $basedir --strip-components=1

WORKDIR $basedir
EXPOSE 8080 7077 6066

ENTRYPOINT ./sbin/start-master.sh && tail -f logs/*.out