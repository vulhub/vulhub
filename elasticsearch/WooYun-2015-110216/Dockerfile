FROM tomcat:8

MAINTAINER phithon <root@leavesongs.com>

RUN set -ex \
    && rm -rf $CATALINA_HOME/webapps/ROOT/* \
    && echo '<% out.println("Hello World! "); %>' | tee $CATALINA_HOME/webapps/ROOT/index.jsp

RUN set -ex \
    && apt-get update \
    && apt-get install -y wget \
    && mkdir -p /usr/share/elasticsearch \
    && wget -qO- https://download.elastic.co/elasticsearch/elasticsearch/elasticsearch-1.5.1.tar.gz \
        | tar zx --strip-components 1 -C /usr/share/elasticsearch

ENV PATH /usr/share/elasticsearch/bin:$PATH

COPY logging.yml /usr/share/elasticsearch/config/

COPY docker-entrypoint.sh /

RUN set -ex \
    && chmod +x /docker-entrypoint.sh \
    && rm -rf /var/lib/apt/lists/* \
    && apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false wget

ENTRYPOINT ["/docker-entrypoint.sh"]

CMD ["catalina.sh", "run"]

EXPOSE 9200 8080