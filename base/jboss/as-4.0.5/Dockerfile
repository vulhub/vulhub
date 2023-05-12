FROM openjdk:7-jre
LABEL maintainer="root <root@leavesongs.com>"

# JBoss version
ENV JBOSS_VERSION 4.0.5.GA
ENV JBOSS_HOME /opt/jboss/jboss4

# default port
EXPOSE 8080

# Installs jboss
RUN cd $HOME && \
    apt-get update && apt-get install -y --no-install-recommends unzip && \
    curl -#SL http://sourceforge.net/projects/jboss/files/JBoss/JBoss-$JBOSS_VERSION/jboss-$JBOSS_VERSION.zip/download -o jboss-$JBOSS_VERSION.zip && \
    unzip -qo jboss-$JBOSS_VERSION.zip && \
    mkdir -p /opt/jboss && \
    mv $HOME/jboss-$JBOSS_VERSION $JBOSS_HOME && \
    rm -rf jboss-$JBOSS_VERSION.zip $JBOSS_HOME/server/all $JBOSS_HOME/server/minimal $JBOSS_HOME/docs

COPY jmx-console/* /opt/jboss/jboss4/server/default/deploy/jmx-console.war/WEB-INF/
COPY web-console/* /opt/jboss/jboss4/server/default/deploy/management/console-mgr.sar/web-console.war/WEB-INF/

CMD ["/opt/jboss/jboss4/bin/run.sh", "-b", "0.0.0.0"]