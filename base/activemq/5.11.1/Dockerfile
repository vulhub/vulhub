FROM vulhub/java:7u21-jdk

ENV ACTIVEMQ_VERSION 5.11.1
ENV ACTIVEMQ apache-activemq-$ACTIVEMQ_VERSION
ENV ACTIVEMQ_TCP=61616 ACTIVEMQ_UI=8161

ENV ACTIVEMQ_HOME /opt/activemq

RUN set -x && \
    curl -s -S https://archive.apache.org/dist/activemq/$ACTIVEMQ_VERSION/$ACTIVEMQ-bin.tar.gz | tar xvz -C /opt && \
    ln -s /opt/$ACTIVEMQ $ACTIVEMQ_HOME

WORKDIR $ACTIVEMQ_HOME
EXPOSE $ACTIVEMQ_TCP $ACTIVEMQ_UI

CMD ["/bin/sh", "-c", "bin/activemq console"]