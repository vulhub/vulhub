FROM python:3.12-slim

LABEL maintainer="phithon <root@leavesongs.com>"

WORKDIR /usr/src
RUN set -ex \
    && pip install uv \
    && uv venv \
    && uv pip install langflow==1.2.0

ENV LANGFLOW_HOST=0.0.0.0 \
    LANGFLOW_AUTO_LOGIN=false \
    LANGFLOW_SUPERUSER=administrator \
    LANGFLOW_SUPERUSER_PASSWORD=vulhub \
    DO_NOT_TRACK=true

CMD ["uv", "run", "langflow", "run"]
