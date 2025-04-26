FROM ubuntu:14.04
LABEL maintainer="phithon <root@leavesongs.com>"

COPY install/ /opt/

RUN set -ex \
    && apt-get update && apt-get install wget libstdc++5 -y \
    && bash /opt/installer.sh \
    && rm -r /var/lib/apt/lists/*

EXPOSE 8500
WORKDIR /opt/coldfusion8

CMD ["bash", "-c", "/opt/coldfusion8/bin/coldfusion start && tail -f /opt/coldfusion8/logs/cfserver.log"]