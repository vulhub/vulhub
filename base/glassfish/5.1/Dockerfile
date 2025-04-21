FROM vulhub/java:8u201-jdk
LABEL maintainer="phithon <root@leavesongs.com>"

ENV GLASSFISH_HOME    /usr/local/glassfish5
ENV PATH $PATH:$GLASSFISH_HOME/bin

RUN set -ex \
    && echo 'deb [trusted=yes] http://archive.debian.org/debian-security stretch/updates main' > /etc/apt/sources.list \
    && echo 'deb [trusted=yes] http://archive.debian.org/debian stretch main' >> /etc/apt/sources.list \
    && apt-get update \
    && apt-get install -y curl unzip zip inotify-tools \
    # && rm -rf /var/lib/apt/lists/* \
    && curl -L -o /tmp/glassfish.zip https://download.eclipse.org/glassfish/glassfish-5.1.0.zip \
    && unzip /tmp/glassfish.zip -d /usr/local \
    && rm -f /tmp/glassfish.zip

COPY docker-entrypoint.sh /entrypoint.sh
RUN set -ex \
    && sed -i 's/address=9009/address=5005/' "${GLASSFISH_HOME}/glassfish/domains/domain1/config/domain.xml" \
    && chmod +x /entrypoint.sh

WORKDIR /usr/local/glassfish5

ENTRYPOINT ["/entrypoint.sh"]

# verbose causes the process to remain in the foreground so that docker can track it
CMD ["asadmin", "start-domain", "--debug=true", "--verbose"]