#!/bin/bash

set -ex
cd /usr/src || exit 1
wait-for-it -t 0 "${MONGO_ADDR}" -- echo "database is ready"

if [[ ! -e "/usr/init.lock" ]]; then
    npm run install-server
fi

if [[ -e "$(pwd)/initdb.js" ]]; then
    node server/app.js &
    sleep 5s
    kill -9 $!
    sleep 1s
    node "$(pwd)/initdb.js"
fi

echo "start server, $*"
exec "$@"
