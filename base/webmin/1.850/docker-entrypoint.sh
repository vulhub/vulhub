#!/bin/bash

set -ex

if [ $# -eq 0 ]; then
    service webmin start
    touch /var/webmin/miniserv.log
    exec tail -f /var/webmin/miniserv.log
fi

exec "$@"