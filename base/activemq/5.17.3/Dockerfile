FROM openjdk:11.0.16-jre

ENV ACTIVEMQ_VERSION 5.17.3
ENV ACTIVEMQ apache-activemq-$ACTIVEMQ_VERSION
ENV ACTIVEMQ_TCP=61616 ACTIVEMQ_AMQP=5672 ACTIVEMQ_STOMP=61613 ACTIVEMQ_MQTT=1883 ACTIVEMQ_WS=61614 ACTIVEMQ_UI=8161

ENV ACTIVEMQ_HOME /opt/activemq

RUN set -x \
    && mkdir -p $ACTIVEMQ_HOME \
    && curl -s -S https://archive.apache.org/dist/activemq/$ACTIVEMQ_VERSION/$ACTIVEMQ-bin.tar.gz | \
        tar xvz --strip-components=1 -C "$ACTIVEMQ_HOME"

RUN set -ex \
    && sed -i 's/127\.0\.0\.1/0.0.0.0/' "${ACTIVEMQ_HOME}/conf/jetty.xml" \
    && echo 'ACTIVEMQ_DEBUG_OPTS="-Xdebug -Xnoagent -Djava.compiler=NONE -Xrunjdwp:transport=dt_socket,server=y,suspend=n,address=*:5005"' >> "$ACTIVEMQ_HOME/bin/env"

WORKDIR $ACTIVEMQ_HOME
EXPOSE $ACTIVEMQ_TCP $ACTIVEMQ_AMQP $ACTIVEMQ_STOMP $ACTIVEMQ_MQTT $ACTIVEMQ_WS $ACTIVEMQ_UI

CMD ["/opt/activemq/bin/activemq", "console"]
