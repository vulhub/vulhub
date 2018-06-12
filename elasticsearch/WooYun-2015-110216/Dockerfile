FROM tomcat:8

MAINTAINER phithon <root@leavesongs.com>

RUN set -ex \
    && rm -rf $CATALINA_HOME/webapps/ROOT/* \
    && echo '<% out.println("Hello World! "); %>' | tee $CATALINA_HOME/webapps/ROOT/index.jsp

RUN set -ex \
    && sed -i 's/deb.debian.org/mirrors.ustc.edu.cn/g' /etc/apt/sources.list \
    && sed -i 's|security.debian.org/debian-security|mirrors.ustc.edu.cn/debian-security|g' /etc/apt/sources.list \
    && apt-get update \
    && apt-get install -y wget \
    && mkdir -p /usr/share/elasticsearch \
    && wget -q https://gitee.com/leehdsniper/vulhub-package/raw/master/elasticsearch-1.5.1.tar.gz \
    && wget -qO- https://download.elastic.co/elasticsearch/elasticsearch/elasticsearch-1.5.1.tar.gz.sha1.txt | sha1sum -c \
    && tar zvxf elasticsearch-1.5.1.tar.gz --strip-components 1 -C /usr/share/elasticsearch \
    && rm elasticsearch-1.5.1.tar.gz

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