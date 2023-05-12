FROM cptactionhank/atlassian-confluence:6.10.2

LABEL maintainer="phithon <root@leavesongs.com>"

USER root

RUN set -ex \
    && mkdir /home/confluence \
    && chown daemon:daemon -R /home/confluence

USER daemon:daemon