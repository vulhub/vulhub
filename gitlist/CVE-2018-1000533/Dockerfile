FROM vulhub/gitlist:0.6.0

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex; \
    mkdir -p /data/example; \
    cd /data/example; \
    echo "# example repository\n\nThis is a example repository for vulhub" > README.md; \
    git init; \
    git config --global user.name "Vulhub"; \
    git config --global user.email "example@vulhub.org"; \
    git add . ; \
    git commit -m "add readme";
