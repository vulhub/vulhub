FROM vulhub/python:2.7

LABEL maintainer="phithon <root@leavesongs.com>"

COPY docker-entrypoint.sh /usr/local/bin/

RUN pip install -U pip \
    && pip install "supervisor==3.3.2" \
    && echo_supervisord_conf | tee /usr/local/etc/supervisord.conf \
    && { \
        echo "[inet_http_server]"; \
        echo "port=0.0.0.0:9001"; \
        echo; \
    } | tee -a /usr/local/etc/supervisord.conf \
    && chmod +x /usr/local/bin/docker-entrypoint.sh

CMD ["/usr/local/bin/docker-entrypoint.sh"]