FROM python:3.7

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends dumb-init openssh-server cron \
    && pip3 install --no-cache-dir salt==3002 pycryptodomex==3.9.9 CherryPy==18.6.0 pyOpenSSL==19.1.0 msgpack==0.6.2 \
    && rm -rf /var/lib/apt/lists/*

RUN set -ex \
    && mkdir -p /etc/pki /etc/salt/pki /etc/salt/minion.d/ /etc/salt/master.d /etc/salt/proxy.d /var/cache/salt /var/log/salt /var/run/salt /run/sshd /root/.ssh

COPY saltinit.py /usr/local/bin/saltinit
ENTRYPOINT ["/usr/bin/dumb-init"]
CMD ["/usr/local/bin/saltinit"]
EXPOSE 22 4505 4506 8000

RUN salt-run salt.cmd tls.create_self_signed_cert
