FROM phusion/baseimage:jammy-1.0.1

LABEL maintainer="phithon <root@leavesongs.com>"

ARG VERSION=3.6.4 BASEDIR=/opt/jumpserver
RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends ca-certificates wget \
    && savedAptMark="$(apt-mark showmanual)" \
    && apt-get install -y --no-install-recommends libcairo2-dev libjpeg-turbo8-dev libpng-dev libtool-bin libossp-uuid-dev \
    libavcodec-dev libavformat-dev libavutil-dev libswscale-dev freerdp2-dev libpango1.0-dev libssh2-1-dev libtelnet-dev \
    libvncserver-dev libwebsockets-dev libpulse-dev libvorbis-dev libwebp-dev libssl-dev gcc make \
    && mkdir -p ${BASEDIR}/guacd \
    && wget -qO- https://archive.apache.org/dist/guacamole/1.5.2/source/guacamole-server-1.5.2.tar.gz | \
        tar xz --strip-components 1 -C ${BASEDIR}/guacd \
    && cd ${BASEDIR}/guacd \
    && ./configure --prefix=/usr/local \
    && make \
    && make install \
    # remove unused libraries
    && apt-mark auto '.*' > /dev/null \
	&& [ -z "$savedAptMark" ] || apt-mark manual $savedAptMark \
    && apt-mark manual $savedAptMark \
    && find /usr/local -type f -executable -exec ldd '{}' 2>/dev/null ';' \
        | awk '/=>/ { so = $(NF-1); if (index(so, "/usr/local/") == 1 || index(so, "/") == 0) { next }; \
            gsub("^/(usr/)?", "", so); print so }' \
        | sort -u \
        | xargs -r dpkg-query --search \
        | cut -d: -f1 \
        | sort -u \
        | xargs -r apt-mark manual \
    \
    && apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false \
    && ldconfig /usr \
    && rm -rf /var/lib/apt/lists/* ${BASEDIR}/guacd

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends gcc g++ make wget ca-certificates openjdk-17-jre-headless nginx sshpass \
        python3 python3-dev python3-pip pkg-config libxmlsec1-dev libpq-dev libffi-dev libxml2-dev libxslt-dev libldap2-dev \
        libsasl2-dev bash-completion libmariadb-dev wait-for-it gettext

COPY requirements.txt /tmp/requirements.txt
WORKDIR ${BASEDIR}

RUN set -ex \
    # download jumpserver core
    && mkdir -p ${BASEDIR}/core \
    && cd ${BASEDIR}/core \
    && wget -qO- https://github.com/jumpserver/jumpserver/releases/download/v${VERSION}/jumpserver-v${VERSION}.tar.gz | \
        tar xz --strip-components 1 \
    && touch config.yml \
    && pip install -r /tmp/requirements.txt \
    && rm -f apps/locale/zh/LC_MESSAGES/django.mo apps/locale/zh/LC_MESSAGES/djangojs.mo \
    && SECRET_KEY=example python3 apps/manage.py compilemessages \
    && rm -rf /tmp/requirements.txt

RUN set -ex \
    # download jumpserver frontend (lina)
    && mkdir -p ${BASEDIR}/lina \
    && wget -qO- https://github.com/jumpserver/lina/releases/download/v${VERSION}/lina-v${VERSION}.tar.gz | \
        tar xz --strip-components 1 -C ${BASEDIR}/lina \
    # download jumpserver terminal frontend (luna)
    && mkdir -p ${BASEDIR}/luna \
    && wget -qO- https://github.com/jumpserver/luna/releases/download/v${VERSION}/luna-v${VERSION}.tar.gz | \
        tar xz --strip-components 1 -C ${BASEDIR}/luna \
    # download jumpserver SSH/SFTP Web Terminal (koko)
    && mkdir -p ${BASEDIR}/koko \
    && wget -qO- https://github.com/jumpserver/koko/releases/download/v${VERSION}/koko-v${VERSION}-linux-amd64.tar.gz | \
        tar xz --strip-components 1 -C ${BASEDIR}/koko \
    && touch config.yml \
    # download jumpserver RDP/VNC client (lion)
    && mkdir -p ${BASEDIR}/lion \
    && wget -qO- https://github.com/jumpserver/lion-release/releases/download/v${VERSION}/lion-v${VERSION}-linux-amd64.tar.gz | \
        tar xz --strip-components 1 -C ${BASEDIR}/lion \
    && touch config.yml \
    # download jumpserver wisp
    && mkdir -p ${BASEDIR}/wisp \
    && wget -qO- https://github.com/jumpserver/wisp/releases/download/v0.1.16/wisp-v0.1.16-linux-amd64.tar.gz | \
        tar xz --strip-components 1 -C ${BASEDIR}/wisp \
    # download jumpserver database connector (magnus)
    && mkdir -p ${BASEDIR}/magnus \
    && wget -qO- https://github.com/jumpserver/magnus-release/releases/download/v${VERSION}/magnus-v${VERSION}-linux-amd64.tar.gz | \
        tar xz --strip-components 1 -C ${BASEDIR}/magnus \
    && touch config.yml \
    # download jumpserver Web DB (chen)
    && mkdir -p ${BASEDIR}/chen \
    && wget -qO- https://github.com/jumpserver/chen-release/releases/download/v${VERSION}/chen-${VERSION}.tar.gz | \
        tar xz --strip-components 1 -C ${BASEDIR}/chen \
    && mkdir -p /etc/service/chen /etc/service/core /etc/service/guacd /etc/service/koko /etc/service/lion /etc/service/magnus /etc/service/nginx /etc/service/celery

COPY ./chen/run /etc/service/chen/run
COPY ./core/run /etc/service/core/run
COPY ./guacd/run /etc/service/guacd/run
COPY ./koko/run /etc/service/koko/run
COPY ./lion/run /etc/service/lion/run
COPY ./magnus/run /etc/service/magnus/run
COPY ./celery/run /etc/service/celery/run
COPY ./nginx/run /etc/service/nginx/run
COPY ./nginx/default.conf /etc/nginx/sites-enabled/default

RUN set -ex \
    && chmod +x /etc/service/chen/run /etc/service/core/run /etc/service/guacd/run /etc/service/koko/run /etc/service/lion/run /etc/service/magnus/run /etc/service/nginx/run /etc/service/celery/run
