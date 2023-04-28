FROM vulhub/java:8u172-jdk

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && echo "deb http://archive.debian.org/debian stretch main contrib non-free" > /etc/apt/sources.list \
    && apt-get update \
    && apt-get install -y --no-install-recommends python3 libfindbin-libs-perl

RUN set -ex \
    && mkdir -p /opt/druid \
    && wget -qO- https://archive.apache.org/dist/druid/25.0.0/apache-druid-25.0.0-bin.tar.gz | tar xz --strip-components 1 -C /opt/druid \
    && cd /opt/druid \
    # fix typo <https://github.com/apache/druid/pull/13657>
    && sed -i 's|mm_task_java_opts_property|mm_task_java_opts_prop|' bin/start-druid-main.py

EXPOSE 8888
WORKDIR /opt/druid
CMD [ "/opt/druid/bin/start-druid", "-c", "conf/druid/single-server/nano-quickstart" ]
