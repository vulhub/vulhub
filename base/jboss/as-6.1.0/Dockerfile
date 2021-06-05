FROM openjdk:7-jre

LABEL maintainer="phithon <root@leavesongs.com>"

# Download jboss from http://jbossas.jboss.org/downloads/
RUN set -ex \
    && curl -L -o jboss.zip http://download.jboss.org/jbossas/6.1/jboss-as-distribution-6.1.0.Final.zip \
    && unzip -q jboss.zip \
    && rm -rf jboss.zip

COPY jmx-console.web.xml /jboss-6.1.0.Final/common/deploy/jmx-console.war/WEB-INF/web.xml
COPY jmx-console.jboss-web.xml /jboss-6.1.0.Final/common/deploy/jmx-console.war/WEB-INF/jboss-web.xml
COPY web-console.web.xml /jboss-6.1.0.Final/common/deploy/jbossws-console.war/WEB-INF/web.xml
COPY web-console.jboss-web.xml /jboss-6.1.0.Final/common/deploy/jbossws-console.war/WEB-INF/jboss-web.xml

COPY run.sh /run.sh
COPY set_jboss_admin_pass.sh /set_jboss_admin_pass.sh
RUN chmod +x /*.sh

EXPOSE 8080
CMD ["/run.sh"]
