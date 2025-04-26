#!/bin/bash

set -ex 

if [ ! -z $ROOT_PASSWORD ]; then
    echo "root:$ROOT_PASSWORD" | chpasswd
fi

exec "$@"