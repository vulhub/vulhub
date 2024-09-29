FROM debian:bookworm

LABEL phith0n="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends build-essential gcc make pkg-config wget ca-certificates bc ghostscript mupdf-tools poppler-utils dbus \
        libexif-dev libglib2.0-dev liblcms2-dev libfreetype6-dev libfontconfig-dev libqpdf-dev libpoppler-cpp-dev libgnutls28-dev libdbus-1-dev libcups2-dev \
        libavahi-glib-dev libavahi-client-dev \
    && mkdir -p /usr/src/libcupsfilters /usr/src/cups-filters /usr/src/libppd /usr/src/cups-browsed /usr/src/cups \
    # install libcupsfilters
    && wget -qO- https://github.com/OpenPrinting/libcupsfilters/releases/download/2.1b1/libcupsfilters-2.1b1.tar.gz | tar xz -C /usr/src/libcupsfilters --strip-components=1 \
    && cd /usr/src/libcupsfilters \
    && CXXFLAGS="-std=c++17" ./configure \
    && make -j$(nproc) \
    && make install \
    # install libppd
    && wget -qO- https://github.com/OpenPrinting/libppd/releases/download/2.1b1/libppd-2.1b1.tar.gz | tar xz -C /usr/src/libppd --strip-components=1 \
    && cd /usr/src/libppd \
    && CXXFLAGS="-std=c++17" ./configure \
    && make -j$(nproc) \
    && make install \
    # install cups-filters
    && wget -qO- https://github.com/OpenPrinting/cups-filters/releases/download/2.0.1/cups-filters-2.0.1.tar.gz | tar xz -C /usr/src/cups-filters --strip-components=1 \
    && cd /usr/src/cups-filters \
    && CXXFLAGS="-std=c++17" ./configure \
    && make -j$(nproc) \
    && make install \
    # install vulnerable cups-browsed
    && wget -qO- https://github.com/OpenPrinting/cups-browsed/releases/download/2.0.1/cups-browsed-2.0.1.tar.gz | tar xz -C /usr/src/cups-browsed --strip-components=1 \
    && cd /usr/src/cups-browsed \
    && CXXFLAGS="-std=c++17" CFLAGS="-g" ./configure \
    && make -j$(nproc) \
    && make install \
    # install CUPS
    && wget -qO-  https://github.com/OpenPrinting/cups/releases/download/v2.4.7/cups-2.4.7-source.tar.gz | tar xz -C /usr/src/cups --strip-components=1 \
    && cd /usr/src/cups \
    && CXXFLAGS="-std=c++17" ./configure \
    && make -j$(nproc) \
    && make install \
    # modify default cups config
    && sed -i 's/Listen localhost:631/Listen 0.0.0.0:631/g' /etc/cups/cupsd.conf \
    && sed -i 's/Order allow,deny/Order allow,deny\n  Allow from all/g' /etc/cups/cupsd.conf \
    # clean
    && rm -rf /usr/src/* \
    && apt-get remove -y --purge build-essential gcc make pkg-config bc \
    && apt-get autoremove -y \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

COPY docker-entrypoint.sh /docker-entrypoint.sh
RUN set -ex \
    && chmod +x /docker-entrypoint.sh

EXPOSE 631/tcp 631/udp
ENTRYPOINT [ "bash", "/docker-entrypoint.sh" ]
CMD [ "cups-browsed", "--debug" ]
