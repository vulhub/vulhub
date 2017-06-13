FROM vulhub/git:2.12.2

MAINTAINER phithon <root@leavesongs.com>

RUN apt-get update \
    && apt-get install -y man less openssh-server --no-install-recommends \
    && rm -r /var/lib/apt/lists/*

RUN mkdir /var/run/sshd \
	&& chmod 0755 /var/run/sshd \
	&& echo 'git:x:33:33:git:/home/git:/bin/bash' >> /etc/passwd \
	&& echo 'git:*:16462:0:99999:7:::' >> /etc/shadow \
	&& echo 'git:x:33:' >> /etc/group \
	&& mkdir -p /home/git/.ssh \
	&& chown 33.33 -R /home/git

ADD authorized_keys /home/git/.ssh/authorized_keys

RUN chown 33.33 /home/git/.ssh/authorized_keys \
    && chmod 0600 /home/git/.ssh/authorized_keys

EXPOSE 22

CMD ["/usr/sbin/sshd", "-D"]