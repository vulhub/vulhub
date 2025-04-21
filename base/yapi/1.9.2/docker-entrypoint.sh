#!/bin/bash

cd /usr/src
wait-for-it -t 0 "${MONGO_ADDR}" -- echo "database is ready"

if [[ ! -e "$(pwd)/init.lock" ]]; then
    npm run install-server
fi

echo "start server, $@"
exec "$@"
