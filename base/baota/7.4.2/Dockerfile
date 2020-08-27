FROM ubuntu:focal

LABEL maintainer="phith0n <root@leavesongs.com>"

ENV DEBIAN_FRONTEND=noninteractive
RUN set -ex \
    && apt-get update && apt-get install -y --no-install-recommends --force-yes \
        ruby lsb-release wget curl libcurl4-openssl-dev gcc make zip \
        unzip openssl libssl-dev gcc libxml2 libxml2-dev libxslt1-dev zlib1g \
         zlib1g-dev libjpeg-dev libpng-dev lsof libpcre3 libpcre3-dev \
         cron net-tools swig build-essential libffi-dev libbz2-dev \
         libncurses-dev libsqlite3-dev libreadline-dev tk-dev libgdbm-dev \
         libdb-dev libdb++-dev libpcap-dev xz-utils git ca-certificates

RUN set -ex \
    && if [ ! -d '/etc/letsencrypt' ]; then \
		mkdir -p /etc/letsencryp; \
		mkdir -p /var/spool/cron; \
		if [ ! -f '/var/spool/cron/crontabs/root' ]; then \
			echo '' > /var/spool/cron/crontabs/root; \
			chmod 600 /var/spool/cron/crontabs/root; \
		fi \
	fi

ADD pip.txt /tmp/pip.txt
RUN set -ex \
    && apt-get install -y --no-install-recommends python3 python3-pip python3-setuptools python3-distutils python3-dev \
    && ln -s /usr/bin/pip3 /usr/bin/pip \
    && ln -s /usr/bin/python3 /usr/bin/python \
    && ln -s /usr/bin/pip3 /usr/bin/btpip \
    && ln -s /usr/bin/python3 /usr/bin/btpython \
    && pip install -U "wheel==0.35.1" \
    && pip install -r /tmp/pip.txt

ARG setup_path="/www"
RUN set -ex \
    && mkdir -p /www/server \
	&& mkdir -p ${setup_path}/server/panel/logs \
	&& mkdir -p ${setup_path}/server/panel/vhost/apache \
	&& mkdir -p ${setup_path}/server/panel/vhost/nginx \
	&& mkdir -p ${setup_path}/server/panel/vhost/rewrite \
	&& mkdir -p ${setup_path}/server/panel/install \
	&& mkdir -p /www/wwwroot \
	&& mkdir -p /www/wwwlogs \
	&& mkdir -p /www/backup/database \
	&& mkdir -p /www/backup/site

ADD public.sh /www/server/panel/install/public.sh
RUN set -ex \
    && panelPort="8888" \
    && cd /tmp \
    && wget -O panel.zip https://github.com/phith0n/baota/archive/release-7.4.2.zip \
    && unzip panel.zip \
    && cp -r baota-release-7.4.2/panel ${setup_path}/server/ \
    && echo "${panelPort}" > ${setup_path}/server/panel/data/port.pl \
    && echo "/admin" > /www/server/panel/data/admin_path.pl

WORKDIR /www/server/panel
CMD ["python", "BT-Panel"]
