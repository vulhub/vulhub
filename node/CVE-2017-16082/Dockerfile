FROM vulhub/node:9.0.0

LABEL maintainer="phithon <root@leavesongs.com>"

COPY package.json package-lock.json /usr/src/

ADD https://raw.githubusercontent.com/vishnubob/wait-for-it/master/wait-for-it.sh /usr/local/bin/wait-for-it.sh

RUN set -ex \
    && cd /usr/src/ \
    && npm install \
    && chmod +x /usr/local/bin/wait-for-it.sh

WORKDIR /usr/src

CMD [ "npm", "run", "start" ]