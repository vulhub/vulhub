FROM vulhub/httpd:2.4

MAINTAINER phithon <root@leavesongs.com>

WORKDIR /tmp

ENV BUILD_TOOLS \
    wget \
    build-essential \
    bison \
    autoconf

RUN apt-get update \
    && apt-get -y install $BUILD_TOOLS \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /usr/local/src/cve-2014-6271 \
    && wget -qO- https://ftp.gnu.org/gnu/bash/bash-4.3.tar.gz \
        | tar zx -C /usr/local/src/cve-2014-6271 --strip-components=1 \
    && cd /usr/local/src/cve-2014-6271 \
    && ./configure --prefix=/usr/local/bash-4.3.0 \
    && make \
    && make install \
    && cd /tmp/ \
    && rm -rf /usr/local/src/cve-2014-6271

COPY ./victim.cgi ${HTTPD_PREFIX}/htdocs/victim.cgi

COPY ./safe.cgi ${HTTPD_PREFIX}/htdocs/safe.cgi

RUN chmod a+x ${HTTPD_PREFIX}/htdocs/*.cgi \
    && apt-get purge -y --auto-remove $BUILD_TOOLS \
    && rm ${HTTPD_PREFIX}/htdocs/index.html

RUN { \
        echo; \
        echo "<Directory \"${HTTPD_PREFIX}/htdocs\">"; \
        echo '  AddHandler cgi-script .cgi'; \
        echo '  AllowOverride None'; \
        echo '  Options Indexes FollowSymLinks ExecCGI'; \
        echo '  Require all granted'; \
        echo '</Directory>'; \
        echo; \
    } >> ${HTTPD_PREFIX}/conf/httpd.conf \
    && sed -i -e "s/\#LoadModule cgi_module/LoadModule cgi_module/g" ${HTTPD_PREFIX}/conf/httpd.conf \
    && sed -i -e "s/\#LoadModule cgid_module/LoadModule cgid_module/g" ${HTTPD_PREFIX}/conf/httpd.conf