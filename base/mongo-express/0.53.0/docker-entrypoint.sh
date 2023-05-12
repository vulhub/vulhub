#!/bin/bash
set -eo pipefail

# if command does not start with mongo-express, run the command instead of the entrypoint
if [ "${1}" != "mongo-express" ]; then
    exec "$@"
fi

# wait for the mongo server to be available
echo Waiting for ${ME_CONFIG_MONGODB_SERVER}:${MONGODB_PORT:-27017}...
wait-for-it.sh "${ME_CONFIG_MONGODB_SERVER}:${MONGODB_PORT:-27017}" -- echo "mongodb is up"

# run mongo-express
exec node app