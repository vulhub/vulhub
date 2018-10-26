FROM vulhub/ghostscript:9.23-python

LABEL maintainer="farisv <fvidyan@gmail.com>"

RUN set -ex \
    && pip install -U pip \
    && pip install "flask==1.0.2" "Pillow==5.3.0"

COPY app.py /usr/src/

WORKDIR /usr/src/

CMD ["python", "app.py"]