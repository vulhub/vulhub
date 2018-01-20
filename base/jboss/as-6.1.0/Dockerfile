FROM vulhub/openjdk:oracle-jdk-6

MAINTAINER phithon <root@leavesongs.com>

# Download jboss from http://jbossas.jboss.org/downloads/
RUN yum install -y unzip wget && \
    yum clean all && \
    wget -q http://download.jboss.org/jbossas/6.1/jboss-as-distribution-6.1.0.Final.zip && \
    unzip -q jboss-as-distribution-6.1.0.Final.zip && \
    rm jboss-as-distribution-6.1.0.Final.zip

ADD jmx-console.web.xml /jboss-6.1.0.Final/common/deploy/jmx-console.war/WEB-INF/web.xml
ADD jmx-console.jboss-web.xml /jboss-6.1.0.Final/common/deploy/jmx-console.war/WEB-INF/jboss-web.xml
ADD web-console.web.xml /jboss-6.1.0.Final/common/deploy/jbossws-console.war/WEB-INF/web.xml
ADD web-console.jboss-web.xml /jboss-6.1.0.Final/common/deploy/jbossws-console.war/WEB-INF/jboss-web.xml

ADD run.sh /run.sh
ADD set_jboss_admin_pass.sh /set_jboss_admin_pass.sh
RUN chmod +x /*.sh

EXPOSE 8080
CMD ["/run.sh"]
