FROM ubuntu:20.04
MAINTAINER ph1sh
EXPOSE 25
ENV DEBIAN_FRONTEND=noninteractive
ENV LANG en_US.utf8
COPY opensmtpd-6.6.1p1.tar.gz /tmp/
WORKDIR /tmp
RUN echo "deb-src http://archive.ubuntu.com/ubuntu/ focal universe">> /etc/apt/sources.list\
&& sed -i "s/archive.ubuntu.com/mirrors.bfsu.edu.cn/g" /etc/apt/sources.list\
&& apt update && apt build-dep opensmtpd -y\
&& tar xf opensmtpd-6.6.1p1.tar.gz\
&& cd /tmp/opensmtpd-6.6.1p1 && ./configure && make && make install\
&& mkdir /var/empty && useradd -c "SMTP Daemon" -d /var/empty -s /sbin/nologin _smtpd && useradd -c "SMTPD Queue" -d /var/empty -s /sbin/nologin _smtpq\
&& echo "smtpd;tail -f /dev/null" > /run.sh
COPY aliases /etc/
COPY smtpd.conf /usr/local/etc/
ENTRYPOINT ["/bin/bash","/run.sh"]
