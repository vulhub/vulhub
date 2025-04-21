FROM python:3.6

ENV ORACLE_HOME=/tmp/instantclient_19_6  LD_LIBRARY_PATH=/tmp/instantclient_19_6  VERSION=19.6.0.0.0 ARCH=x86_64

RUN set -ex \
    && apt-get update && apt-get upgrade -y \
    && apt-get install gcc g++ make binutils libproj-dev gdal-bin libaio1 libaio-dev unzip -y

ADD https://raw.githubusercontent.com/vishnubob/wait-for-it/master/wait-for-it.sh /usr/local/bin/wait-for-it.sh

RUN  set -ex \
	&& cd /tmp/ \
	&& wget download.oracle.com/otn_software/linux/instantclient/19600/instantclient-basiclite-linux.x64-19.6.0.0.0dbru.zip  \
	&& wget download.oracle.com/otn_software/linux/instantclient/19600/instantclient-sdk-linux.x64-19.6.0.0.0dbru.zip \
	&& unzip instantclient-basiclite-linux.x64-19.6.0.0.0dbru.zip && unzip instantclient-sdk-linux.x64-19.6.0.0.0dbru.zip \
	&& rm *.zip

RUN set -ex \
	&& pip install -U pip \
	&& pip install cx_Oracle==5.3 django==3.0.3 \
	&& chmod +x /usr/local/bin/wait-for-it.sh

