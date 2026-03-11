FROM node:22-slim

LABEL maintainer="phithon <root@leavesongs.com>"

SHELL ["/bin/bash", "-o", "pipefail", "-c"]

WORKDIR /code
RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends curl ca-certificates \
    && curl -#sSL https://github.com/chartbrew/chartbrew/archive/refs/tags/v4.8.0.tar.gz | tar xz --strip-components=1 -C /code \
    && cd client && npm install \
    && cd ../server && npm install \
    && cd .. && npm run prepareSettings \
    && cd client && npm run build \
    && apt-get purge -y curl \
    && apt-get autoremove -y \
    && rm -rf /var/lib/apt/lists/* /root/.npm /tmp/*
EXPOSE 4018 4019
ENTRYPOINT ["./entrypoint.sh"]
