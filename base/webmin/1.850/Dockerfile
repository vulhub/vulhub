FROM buildpack-deps:jessie-curl

COPY docker-entrypoint.sh /docker-entrypoint.sh

RUN apt-get -o Acquire::GzipIndexes=false update \
    && apt-get install -y perl libnet-ssleay-perl openssl libauthen-pam-perl libpam-runtime libio-pty-perl python apt-show-versions \
    && cd /tmp/ \
    && wget http://prdownloads.sourceforge.net/webadmin/webmin_1.850_all.deb \
    && dpkg -i webmin_1.850_all.deb \
    && echo 'root:password' | chpasswd \
    && chmod +x /docker-entrypoint.sh \
    && rm -rf /tmp/* /var/lib/apt/lists/*

ENTRYPOINT /docker-entrypoint.sh