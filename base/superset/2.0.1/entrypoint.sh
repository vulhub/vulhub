#!/bin/bash

set -eo pipefail

load_examples_async() {
    echo "Waiting for Superset to be ready..."
    until curl -s --max-time 1 http://localhost:8088/health > /dev/null; do
        sleep 1
        echo -n "."
    done
    echo "Loading examples..."
    superset load_examples
}

if [[ ! -e /app/superset_home/superset.db ]]; then
    echo "Database not found, initializing..."
    superset db upgrade

    echo "Creating admin user..."
    superset fab create-admin \
              --username ${SUPERSET_ADMIN_USERNAME:-admin} \
              --firstname Superset \
              --lastname Admin \
              --email ${SUPERSET_ADMIN_EMAIL:-admin@superset.com} \
              --password ${SUPERSET_ADMIN_PASSWORD:-vulhub}

    echo "Initializing Superset..."
    superset init
fi

if [[ "${SUPERSET_LOAD_EXAMPLES}" == "yes" ]]; then
    # Start loading examples in background
    load_examples_async &
fi

exec "$@"
