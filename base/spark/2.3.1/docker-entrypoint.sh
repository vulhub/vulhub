#!/usr/bin/env bash

set -ex

if [ $# -lt 1 ]; then
    echo "Usage: $0 master|slave params..."
    exit
fi

if [ $1 == "master" ]; then
    sbin/start-master.sh
elif [ $1 == "slave" ]; then
    sbin/start-slave.sh ${@:2}
else
    exec "$@"
fi

exec tail -f logs/*.out
