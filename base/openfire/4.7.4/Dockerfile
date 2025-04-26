FROM openjdk:17.0.2-slim

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends wget ca-certificates \
    && mkdir /mnt/openfire \
    && wget -qO- https://github.com/igniterealtime/Openfire/releases/download/v4.7.4/openfire_4_7_4.tar.gz | tar xz -C /mnt/openfire --strip-components=1

COPY openfire.xml /mnt/openfire/conf/openfire.xml
EXPOSE 9090
WORKDIR /mnt/openfire

CMD [ "/mnt/openfire/bin/openfire", "run" ]
