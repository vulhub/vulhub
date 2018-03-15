FROM buildpack-deps:stretch-curl
LABEL maintainer="phithon <root@leavesongs.com>"

ADD ./build/install/ /tmp/
RUN set -ex \
    && bash /tmp/install-cf11.sh

EXPOSE 8500
WORKDIR /opt/coldfusion11

CMD ["bash", "-c", "/opt/coldfusion11/cfusion/bin/coldfusion start && tail -f /opt/coldfusion11/cfusion/logs/coldfusion-out.log"]