FROM node:22-slim

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends curl openssh-client ca-certificates socat git

RUN set -ex \
    && npm install -g clawdbot@2026.1.24-3 \
    && ln -sf /usr/local/lib/node_modules/clawdbot /usr/local/lib/node_modules/openclaw \
    && ln -sf /usr/local/bin/clawdbot /usr/local/bin/openclaw \
    && rm -rf /tmp/* /root/.npm

COPY docker-entrypoint.sh /docker-entrypoint.sh
RUN chmod +x /docker-entrypoint.sh

EXPOSE 18789

ENTRYPOINT ["/docker-entrypoint.sh"]
