FROM debian:9

LABEL maintainer="phithon <root@leavesongs.com>"

RUN apt-get update && apt-get install -y \
        autoconf \
        automake \
        build-essential \
        cmake \
        git-core \
        libass-dev \
        libfreetype6-dev \
        libsdl2-dev \
        libtool \
        libva-dev \
        libvdpau-dev \
        libvorbis-dev \
        libxcb1-dev \
        libxcb-shm0-dev \
        libxcb-xfixes0-dev \
        pkg-config \
        texinfo \
        wget \
        zlib1g-dev \
    && wget -qO- https://www.ffmpeg.org/releases/ffmpeg-3.2.4.tar.gz | tar --strip-components 1 -xz -C /usr/src \
    && cd /usr/src \
    && ./configure --pkg-config-flags="--static" --disable-yasm \
    && make && make install \
    && rm -rf /usr/src/*

CMD ["ffmpeg"]
