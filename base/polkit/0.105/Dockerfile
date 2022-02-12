FROM ubuntu:focal

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends wget ca-certificates make gcc libc-dev \
        # dependencies for policykit
        libglib2.0-0 libpam-systemd dbus

RUN set -ex \
    && cd /tmp \
    && wget http://archive.ubuntu.com/ubuntu/pool/main/p/policykit-1/libpolkit-backend-1-0_0.105-14.1ubuntu0.5_amd64.deb \
    && wget http://archive.ubuntu.com/ubuntu/pool/main/p/policykit-1/policykit-1_0.105-14.1ubuntu0.5_amd64.deb \
    && wget http://archive.ubuntu.com/ubuntu/pool/main/p/policykit-1/libpolkit-gobject-1-0_0.105-14.1ubuntu0.5_amd64.deb \
    && wget http://archive.ubuntu.com/ubuntu/pool/main/p/policykit-1/libpolkit-agent-1-0_0.105-14.1ubuntu0.5_amd64.deb \
    && dpkg -i libpolkit-gobject-1-0_0.105-14.1ubuntu0.5_amd64.deb \
    && dpkg -i libpolkit-backend-1-0_0.105-14.1ubuntu0.5_amd64.deb \
    && dpkg -i libpolkit-agent-1-0_0.105-14.1ubuntu0.5_amd64.deb \
    && dpkg -i policykit-1_0.105-14.1ubuntu0.5_amd64.deb \
    && rm -rf /tmp/*.deb

USER nobody
