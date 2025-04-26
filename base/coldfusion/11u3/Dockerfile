FROM buildpack-deps:stretch-curl
LABEL maintainer="phithon <root@leavesongs.com>"

COPY ./build/install/ /tmp/
RUN set -ex \
    && apt-get update && apt-get install --no-install-recommends -y procps \
    && bash /tmp/install-cf11.sh \
    && rm -r /var/lib/apt/lists/* /tmp/*

EXPOSE 8500
WORKDIR /opt/coldfusion11

CMD ["bash", "-c", "/opt/coldfusion11/cfusion/bin/coldfusion start && tail -f /opt/coldfusion11/cfusion/logs/coldfusion-out.log"]