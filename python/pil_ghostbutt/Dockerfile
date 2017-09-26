FROM python:3.6

MAINTAINER neargle <nearg1e.com@gmail.com>

RUN set -x \
    && apt-get update \
    && apt-get install -y wget  \
    && pip install --upgrade pip \
    && pip install flask Pillow 

# local

ARG GS_URL=ghostscript-9.21-linux-x86_64.tgz
ADD $GS_URL /tmp/
RUN mkdir -p /opt/ghostscript \
    && mv /tmp/ghostscript-9.21-linux-x86_64/gs-921-linux-x86_64 /usr/local/bin/gs

# remote

# ARG GS_URL=https://github.com/ArtifexSoftware/ghostpdl-downloads/releases/download/gs921/ghostscript-9.21-linux-x86_64.tgz
# ADD $GS_URL /tmp/gs.tgz
# RUN mkdir -p /opt/ghostscript \
#     && tar zxf /tmp/gs.tgz -C /opt/ghostscript --strip-components=1 \
#     && mv /opt/ghostscript/gs-921-linux-x86_64 /usr/local/bin/gs

ARG PY_SRC=src/
ADD $PY_SRC /tmp/src

WORKDIR /tmp/src
EXPOSE 8080

CMD [ "sh", "-c", "python app.py" ]