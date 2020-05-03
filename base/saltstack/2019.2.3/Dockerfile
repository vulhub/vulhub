FROM python:3.7

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends dumb-init \
    && rm -rf /var/lib/apt/lists/*

ADD saltinit.py /usr/local/bin/saltinit
RUN addgroup --gid 450 --system salt && adduser --shell /bin/bash --system --disabled-password --ingroup salt salt && \
    mkdir -p /etc/pki /etc/salt/pki /etc/salt/minion.d/ /etc/salt/master.d /etc/salt/proxy.d /var/cache/salt /var/log/salt /var/run/salt && \
    chmod -R 2775 /etc/pki /etc/salt /var/cache/salt /var/log/salt /var/run/salt && \ 
    chgrp -R salt /etc/pki /etc/salt /var/cache/salt /var/log/salt /var/run/salt \
    && chmod +x /usr/local/bin/saltinit

ENTRYPOINT ["/usr/bin/dumb-init"]
CMD ["/usr/local/bin/saltinit"]
EXPOSE 4505 4506 8000

RUN pip3 install --no-cache-dir salt==2019.2.3 pycryptodomex CherryPy pyOpenSSL
RUN su - salt -c 'salt-run salt.cmd tls.create_self_signed_cert'
