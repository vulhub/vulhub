FROM buildpack-deps:xenial AS build-env
LABEL maintainer="sayantsing@gmail.com"

WORKDIR /root/

ENV MYSQL_VERSION 5.5.23

RUN wget -qO- https://ftp.gnu.org/gnu/m4/m4-1.4.9.tar.gz | tar zx && \
    wget -qO- https://ftp.gnu.org/gnu/bison/bison-2.5.1.tar.gz | tar zx && \
    wget -qO- https://github.com/mysql/mysql-server/archive/mysql-${MYSQL_VERSION}.tar.gz | tar zx

RUN set -ex \
    && cd /root/m4-1.4.9 \
    && ./configure \
    && make \
    && make install \
    && rm -rf /root/m4-1.4.9

RUN set -ex \
    && cd /root/bison-2.5.1 \
    && ./configure \
    && make \
    && make install \
    && rm -rf /root/bison-2.5.1

RUN set -ex && \
    apt-get update && \
    apt-get install -y --no-install-recommends cmake && \
    cd /root/mysql-server-mysql-${MYSQL_VERSION} && \
    cmake . && make && make install && \
    rm -rf /root/mysql-server-mysql-${MYSQL_VERSION}

FROM ubuntu:xenial
LABEL maintainer="phithon <root@leavesongs.com>"

COPY --from=build-env /usr/local/mysql /usr/local/mysql

RUN set -ex && \
    groupadd mysql && useradd -r -g mysql -s /bin/false mysql && \
    mkdir /usr/local/mysql/mysql-files && \
    chmod 750 /usr/local/mysql/mysql-files && \
    chown -R mysql /usr/local/mysql && \
    chgrp -R mysql /usr/local/mysql && \
    ln -s /usr/local/mysql/bin/mysqld_safe /usr/local/bin && \
    ln -s /usr/local/mysql/bin/mysqladmin /usr/local/bin && \
    ln -s /usr/local/mysql/bin/mysql /usr/local/bin && \
    cp /usr/local/mysql/support-files/mysql.server /etc/init.d/mysql && \
    /usr/local/mysql/scripts/mysql_install_db --user=mysql --basedir=/usr/local/mysql --datadir=/usr/local/mysql/data && \
    service mysql start && \
    mysql -uroot -e "UPDATE mysql.user SET password=PASSWORD('123456') WHERE User='root';" && \ 
    mysql -uroot -e "GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY '123456' WITH GRANT OPTION;" && \
    mysql -uroot -e "FLUSH PRIVILEGES;"

EXPOSE 3306

CMD ["/bin/bash", "-c", "mysqld_safe"]