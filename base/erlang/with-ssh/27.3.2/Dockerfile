FROM erlang:27.3.2-slim

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends openssh-client \
    && mkdir -p /etc/ssh \
    && ssh-keygen -t rsa -f /etc/ssh/ssh_host_rsa_key -N ""

COPY start_ssh.escript /usr/local/bin/start_ssh.escript

CMD ["escript", "/usr/local/bin/start_ssh.escript"]
