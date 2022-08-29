FROM ubuntu:focal

LABEL Vendor="Apache.org" License="ApacheV2" Version="4.17.0.0"

ARG DEBIAN_FRONTEND=noninteractive

WORKDIR /root
RUN apt-get -y update && apt-get install -y \
    genisoimage \
    libffi-dev \
    libssl-dev \
    curl \
    wget \
    gcc-10 \
    git \
    sudo \
    ipmitool \
    iproute2 \
    maven \
    openjdk-11-jdk \
    python3-dev \
    python-is-python3 \
    python3-setuptools \
    python3-pip \
    python3-mysql.connector \
    supervisor

RUN apt-get install -qqy mysql-server && \
    apt-get clean all && \
    mkdir /var/run/mysqld; \
    chown mysql /var/run/mysqld

RUN echo '''sql_mode = "STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION"''' >> /etc/mysql/mysql.conf.d/mysqld.cnf

COPY supervisord.conf /etc/supervisor/conf.d/supervisord.conf
RUN set -ex \
    && wget -qO- https://github.com/apache/cloudstack/archive/refs/tags/4.17.0.0.tar.gz | tar xz --strip-components=1 -C /root

RUN mvn -Pdeveloper -Dsimulator -DskipTests clean install

RUN find /var/lib/mysql -type f -exec touch {} \; && \
    (/usr/bin/mysqld_safe &) && \
    sleep 5; \
    mysql -e "ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password by ''" --connect-expired-password; \
    mvn -Pdeveloper -pl developer -Ddeploydb; \
    mvn -Pdeveloper -pl developer -Ddeploydb-simulator; \
    MARVIN_FILE=`find /root/tools/marvin/dist/ -name "Marvin*.tar.gz"`; \
    rm -rf /usr/bin/x86_64-linux-gnu-gcc && \
    ln -s /usr/bin/gcc-10 /usr/bin/x86_64-linux-gnu-gcc; \
    pip3 install $MARVIN_FILE

RUN curl -sL https://deb.nodesource.com/setup_12.x | sudo -E bash -; \
    apt-get install -y nodejs; \
    cd ui && npm rebuild node-sass && npm install

VOLUME /var/lib/mysql

EXPOSE 8080 8096 5050

CMD ["/usr/bin/supervisord"]
