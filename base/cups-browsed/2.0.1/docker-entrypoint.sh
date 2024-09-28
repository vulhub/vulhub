#!/bin/bash

set -e

if [ "$1" = "cups" ]; then
    /etc/init.d/dbus start
    exec cupsd -f
elif [ "$1" = "cups-browsed" ]; then
    /etc/init.d/dbus start
    cupsd
    exec "$@"
else
    exec "$@"
fi
