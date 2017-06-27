FROM vulhub/ffmpeg:3.0.8

MAINTAINER phithon <root@leavesongs.com>

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends php-cli

COPY ./www /usr/local/src

WORKDIR /usr/local/src

ENTRYPOINT ["bash"]

CMD ["php", "-S", "0.0.0.0:8080"]
