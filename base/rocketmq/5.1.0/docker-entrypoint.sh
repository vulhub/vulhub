#!/bin/bash
set -e

ROCKETMQ_HOME="$(pwd)"
if [[ "$1" == mq* ]]; then
    exec "$ROCKETMQ_HOME/bin/$1" "${@:2}"
else
    exec "$@"
fi