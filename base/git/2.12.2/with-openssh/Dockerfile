FROM vulhub/git:2.12.2

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install --no-install-recommends -y man less openssh-server \
    && rm -r /var/lib/apt/lists/*

RUN set -ex \
    && mkdir /var/run/sshd \
	&& chmod 0755 /var/run/sshd \
	&& echo 'git:x:33:33:git:/home/git:/bin/bash' >> /etc/passwd \
	&& echo 'git:*:16462:0:99999:7:::' >> /etc/shadow \
	&& echo 'git:x:33:' >> /etc/group \
	&& mkdir -p /home/git/.ssh \
	&& chown 33.33 -R /home/git

COPY authorized_keys /home/git/.ssh/authorized_keys

RUN chown 33.33 /home/git/.ssh/authorized_keys \
    && chmod 0600 /home/git/.ssh/authorized_keys

EXPOSE 22

CMD ["/usr/sbin/sshd", "-D"]