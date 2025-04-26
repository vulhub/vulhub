FROM node:14

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && cd /usr/src \
    && wget -qO- https://github.com/YMFE/yapi/archive/refs/tags/v1.10.2.tar.gz | tar zx --strip-components=1 \
    && cp config_example.json ../config.json \
    && sed -i 's/npmrepo.corp.qunar.com/registry.npm.taobao.org/g' package-lock.json

RUN set -ex \
    && cd /usr/src \
    && yarn

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends wait-for-it \
    && rm -rf /var/lib/apt/lists/*

COPY docker-entrypoint.sh /docker-entrypoint.sh
WORKDIR /usr/src
ENTRYPOINT ["bash", "/docker-entrypoint.sh"]
CMD [ "npm", "start" ]
