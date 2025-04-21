FROM buildpack-deps:jessie-curl

COPY debian.patch /debian.patch

RUN set -ex \
    && apt-get update \
    && apt-get install --no-install-recommends -y gcc make libc6-dev patch \
    && wget -qO- http://www.acme.com/software/mini_httpd/mini_httpd-1.29.tar.gz \
        | tar zx -C /usr/src --strip-components=1 \
    && cd /usr/src \
    && patch mini_httpd.c < /debian.patch \
    && make \
    && make install \
    && mkdir -p /var/www/html \
    && chown www-data:www-data /var/www/html \
    && rm -rf /usr/src/* /var/lib/apt/lists/* /debian.patch

WORKDIR /var/www/html
EXPOSE 8080

CMD ["mini_httpd", "-p", "8080", "-u", "www-data", "-h", "0.0.0.0", "-d", "/var/www/html", "-D", "-c", "**.cgi", "-l", "/dev/stdout", "-T", "utf-8"]
