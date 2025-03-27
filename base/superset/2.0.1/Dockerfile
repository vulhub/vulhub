FROM apache/superset:2.0.1

LABEL maintainer="phithon <root@leavesongs.com>"

USER root
COPY ./entrypoint.sh /entrypoint.sh
COPY ./run-server.sh /run-server.sh

ENTRYPOINT ["/entrypoint.sh"]
CMD ["/run-server.sh", "app-gunicorn"]
