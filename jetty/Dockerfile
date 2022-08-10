FROM jetty:9.4.9
RUN mkdir /var/lib/jetty/webapps/root
RUN bash -c 'set -ex \
  && cd /var/lib/jetty/webapps/root \
  && wget https://tomcat.apache.org/tomcat-7.0-doc/appdev/sample/sample.war \
  && unzip sample.war'
EXPOSE 8080
ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["java","-jar","/usr/local/jetty/start.jar"]