FROM debian:buster

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends wget ca-certificates \
    && mkdir -p /opt/bin \
    && wget -O /opt/bin/minio https://dl.min.io/server/minio/release/linux-amd64/archive/minio.RELEASE.2023-02-27T18-10-45Z \
    && wget -O /opt/bin/mc https://dl.min.io/client/mc/release/linux-amd64/archive/mc.RELEASE.2023-02-28T00-12-59Z

ENV MINIO_ACCESS_KEY_FILE=access_key \
    MINIO_SECRET_KEY_FILE=secret_key \
    MINIO_ROOT_USER_FILE=access_key \
    MINIO_ROOT_PASSWORD_FILE=secret_key \
    MINIO_KMS_SECRET_KEY_FILE=kms_master_key \
    MINIO_CONFIG_ENV_FILE=config.env \
    PATH=/opt/bin:$PATH

COPY docker-entrypoint.sh /opt/bin/docker-entrypoint.sh
RUN set -ex \
    && chmod +x /opt/bin/* \
    && mkdir -p /mnt/data

EXPOSE 9000 9001
ENTRYPOINT [ "/opt/bin/docker-entrypoint.sh" ]
CMD ["minio", "server", "--console-address", ":9001", "/mnt/data"]
