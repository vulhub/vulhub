FROM golang:1.10-alpine3.7 AS build-env

ARG TAGS="sqlite"
ENV TAGS "bindata $TAGS"

#Build deps
RUN apk --no-cache add build-base git

#Setup repo
RUN git clone -b v1.4.0 https://github.com/go-gitea/gitea ${GOPATH}/src/code.gitea.io/gitea
WORKDIR ${GOPATH}/src/code.gitea.io/gitea

#Checkout version if set
RUN make clean generate build

FROM alpine:3.7
LABEL maintainer="maintainers@gitea.io"

EXPOSE 22 3000

RUN apk --no-cache add \
    bash \
    ca-certificates \
    curl \
    gettext \
    git \
    linux-pam \
    openssh \
    s6 \
    sqlite \
    su-exec \
    tzdata

RUN addgroup \
    -S -g 1000 \
    git && \
  adduser \
    -S -H -D \
    -h /data/git \
    -s /bin/bash \
    -u 1000 \
    -G git \
    git && \
  echo "git:$(dd if=/dev/urandom bs=24 count=1 status=none | base64)" | chpasswd

ENV USER git
ENV GITEA_CUSTOM /data/gitea

ENTRYPOINT ["/usr/bin/entrypoint"]
CMD ["/bin/s6-svscan", "/etc/s6"]

COPY --from=build-env /go/src/code.gitea.io/gitea/docker /
COPY --from=build-env /go/src/code.gitea.io/gitea/gitea /app/gitea/gitea
RUN ln -s /app/gitea/gitea /usr/local/bin/gitea