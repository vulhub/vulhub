#!/bin/bash

set -ex
cd /usr/src

wait-for-it.sh -t 0 db:5432 -- echo "database is up"

python app.py migrate

exec "$@"
