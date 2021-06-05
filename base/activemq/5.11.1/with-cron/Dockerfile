FROM vulhub/activemq:5.11.1

LABEL maintainer="phithon <root@leavesongs.com>"

RUN apt-get update \
    && apt-get install -y cron rsyslog --no-install-recommends \
    && rm -r /var/lib/apt/lists/*

COPY entrypoint.sh /usr/local/bin/

CMD ["/bin/sh", "/usr/local/bin/entrypoint.sh"]