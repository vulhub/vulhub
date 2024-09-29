#!/bin/bash

set -e

if [ "$1" = "cups" ]; then
    /etc/init.d/dbus start
elif [ "$1" = "cups-browsed" ]; then
    /etc/init.d/dbus start
    cupsd
fi

exec "$@"
