FROM vulhub/libssh:0.6.4

LABEL maintainer="phithon <root@leavesongs.com>"

# gcc for cgo
RUN apt-get update && apt-get install -y --no-install-recommends \
		g++ \
		gcc \
		libc6-dev \
		make \
		pkg-config \
	&& rm -rf /var/lib/apt/lists/*

ENV GOLANG_VERSION 1.11.1

RUN set -eux; \
	\
# this "case" statement is generated via "update.sh"
	dpkgArch="$(dpkg --print-architecture)"; \
	case "${dpkgArch##*-}" in \
		amd64) goRelArch='linux-amd64'; goRelSha256='2871270d8ff0c8c69f161aaae42f9f28739855ff5c5204752a8d92a1c9f63993' ;; \
		armhf) goRelArch='linux-armv6l'; goRelSha256='bc601e428f458da6028671d66581b026092742baf6d3124748bb044c82497d42' ;; \
		arm64) goRelArch='linux-arm64'; goRelSha256='25e1a281b937022c70571ac5a538c9402dd74bceb71c2526377a7e5747df5522' ;; \
		i386) goRelArch='linux-386'; goRelSha256='52935db83719739d84a389a8f3b14544874fba803a316250b8d596313283aadf' ;; \
		ppc64el) goRelArch='linux-ppc64le'; goRelSha256='f929d434d6db09fc4c6b67b03951596e576af5d02ff009633ca3c5be1c832bdd' ;; \
		s390x) goRelArch='linux-s390x'; goRelSha256='93afc048ad72fa2a0e5ec56bcdcd8a34213eb262aee6f39a7e4dfeeb7e564c9d' ;; \
		*) goRelArch='src'; goRelSha256='558f8c169ae215e25b81421596e8de7572bd3ba824b79add22fba6e284db1117'; \
			echo >&2; echo >&2 "warning: current architecture ($dpkgArch) does not have a corresponding Go binary release; will be building from source"; echo >&2 ;; \
	esac; \
	\
	url="https://golang.org/dl/go${GOLANG_VERSION}.${goRelArch}.tar.gz"; \
	wget -O go.tgz "$url"; \
	echo "${goRelSha256} *go.tgz" | sha256sum -c -; \
	tar -C /usr/local -xzf go.tgz; \
	rm go.tgz; \
	\
	if [ "$goRelArch" = 'src' ]; then \
		echo >&2; \
		echo >&2 'error: UNIMPLEMENTED'; \
		echo >&2 'TODO install golang-any from jessie-backports for GOROOT_BOOTSTRAP (and uninstall after build)'; \
		echo >&2; \
		exit 1; \
	fi; \
	\
	export PATH="/usr/local/go/bin:$PATH"; \
	go version

ENV GOPATH /go
ENV PATH $GOPATH/bin:/usr/local/go/bin:$PATH

RUN mkdir -p "$GOPATH/src" "$GOPATH/bin" && chmod -R 777 "$GOPATH"
WORKDIR $GOPATH