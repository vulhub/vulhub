FROM debian:bullseye

RUN apt-get update && \
    apt-get install -y wget build-essential xinetd xz-utils && \
    rm -rf /var/lib/apt/lists/*

# Download and compile vulnerable version of inetutils (2.5)
RUN cd /tmp && \
    wget https://ftp.gnu.org/gnu/inetutils/inetutils-2.5.tar.xz && \
    tar -xf inetutils-2.5.tar.xz && \
    cd inetutils-2.5 && \
    ./configure --prefix=/usr --disable-clients && \
    make && \
    make install && \
    rm -rf /tmp/inetutils*

# Create necessary users and files for telnet
RUN useradd -m -s /bin/bash testuser && \
    echo "testuser:testuser" | chpasswd && \
    echo "root:root" | chpasswd

# Configure xinetd for telnetd
COPY telnet.xinetd /etc/xinetd.d/telnet

EXPOSE 23

CMD ["/usr/sbin/xinetd", "-dontfork"]
