FROM ubuntu:14.04
LABEL maintainer="phithon <root@leavesongs.com>"

ADD install/ /opt/

RUN set -ex \
    && apt-get update && apt-get install wget libstdc++5 -y \
    && bash /opt/installer.sh \
    && rm -r /var/lib/apt/lists/*

EXPOSE 8500

CMD ["bash", "-c", "/opt/coldfusion11/cfusion/bin/coldfusion start && tail -f /opt/coldfusion11/cfusion/logs/coldfusion-out.log"]