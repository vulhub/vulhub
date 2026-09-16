FROM python:3.12-slim

LABEL maintainer="phithon <root@leavesongs.com>"

WORKDIR /usr/src
# `langflow` is a 6KB metapackage whose only job is to pull in
# langflow-base[complete] -- 111 integration deps including four separate
# torch consumers (~5GB of CUDA libraries). The application code all lives
# in langflow-base, so we install it directly and only take the version stub
# and the `langflow run` entry point from the metapackage, which keeps the
# image below 1GB. `openai` is hard-imported by the voice-mode API at
# startup but is not part of langflow-base's core dependencies. langflow-base
# is pinned to 0.8.4 -- what the metapackage's `~=0.8.1` requirement resolves
# to; older 0.8.x patches lack the /build_public_tmp/{job_id}/events route.
RUN set -ex \
    && pip install --no-cache-dir uv \
    && uv venv \
    && uv pip install --no-cache langflow-base==0.8.4 "openai>=1.68.2,<2.0.0" \
    && uv pip install --no-cache --no-deps langflow==1.8.1 \
    && rm -rf /root/.cache/uv

# Model a realistic deployment: authentication is enabled (AUTO_LOGIN off)
# and the administrator has shared one flow through the playground feature.
# Langflow imports the JSON into the database at startup via
# LANGFLOW_LOAD_FLOWS_PATH, keeping the fixed flow UUID below stable.
COPY public-flow.json /app/flows/public-flow.json

ENV LANGFLOW_HOST=0.0.0.0 \
    LANGFLOW_AUTO_LOGIN=false \
    LANGFLOW_SUPERUSER=administrator \
    LANGFLOW_SUPERUSER_PASSWORD=vulhub \
    LANGFLOW_LOAD_FLOWS_PATH=/app/flows \
    DO_NOT_TRACK=true

CMD ["uv", "run", "langflow", "run"]
