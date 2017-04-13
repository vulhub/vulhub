FROM httpd
MAINTAINER Medici.Yan@Gmail.com

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

RUN mkdir -p /usr/local/src/cve-2014-7169 \
    && wget -qO- https://ftp.gnu.org/gnu/bash/bash-4.3.tar.gz \
        | tar zx -C /usr/local/src/cve-2014-7169 --strip-components=1 \
    && cd /usr/local/src/cve-2014-7169 \
    && wget -qO- $(seq -f "https://ftp.gnu.org/gnu/bash/bash-4.3-patches/bash43-%03g" 25) | patch -p0 \
    && ./configure --prefix=/usr/local/bash-4.3.25 \
    && make \
    && make install \
    && cd /tmp/ \
    && rm -rf /usr/local/src/cve-2014-7169

COPY ./cgi-bin/ /usr/local/apache2/cgi-bin/

RUN chmod a+x /usr/local/apache2/cgi-bin/* \
    && apt-get purge -y --auto-remove $BUILD_TOOLS

RUN { \
        echo; \
        echo '<Directory "/usr/local/apache2/cgi-bin">'; \
        echo '  SetHandler cgi-script'; \
        echo '  AllowOverride None'; \
        echo '  Options +ExecCGI +Indexes'; \
        echo '  Require all granted'; \
        echo '</Directory>'; \
        echo; \
    } >> /usr/local/apache2/conf/httpd.conf \
    && sed -i -e "s/\#LoadModule cgi_module/LoadModule cgi_module/g" /usr/local/apache2/conf/httpd.conf \
    && sed -i -e "s/\#LoadModule cgid_module/LoadModule cgid_module/g" /usr/local/apache2/conf/httpd.conf