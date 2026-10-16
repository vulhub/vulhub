FROM filebrowser/filebrowser:v2.63.5@sha256:aefb0c20de10ef8b617995ca5522479ad40d41e6386bd01946a345c6026ff31c

LABEL maintainer="phithon <root@leavesongs.com>"

COPY --chmod=755 docker-entrypoint.sh /docker-entrypoint.sh

ENV FB_ADDRESS=127.0.0.1 \
    FB_PORT=80

ENTRYPOINT ["/bin/tini", "--", "/docker-entrypoint.sh"]
