FROM node:20-slim

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends curl \
    && npm install -g n8n@1.65.0 \
    && rm -rf /var/lib/apt/lists/*

COPY docker-entrypoint.sh /docker-entrypoint.sh
RUN chmod +x /docker-entrypoint.sh

ENV N8N_SECURE_COOKIE=false

EXPOSE 5678

ENTRYPOINT ["/docker-entrypoint.sh"]
