#!/bin/bash

set -ex

echo "Waiting for database to start..."
wait-for-it -t 0 "${MYSQL_HOST:-127.0.0.1}:${MYSQL_PORT:-3306}" -- echo "Daabase is up"

exec "$@"
