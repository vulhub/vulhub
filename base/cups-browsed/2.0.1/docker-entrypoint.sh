#!/bin/bash

set -e

if [ "$1" = "cups" ]; then
    exec cupsd -f -c /etc/cups/cupsd.conf -s /etc/cups/cups-files.conf
elif [ "$1" = "cups-browsed" ]; then
    cupsd -c /etc/cups/cupsd.conf -s /etc/cups/cups-files.conf
    exec cups-browsed -c /etc/cups/cups-browsed.conf --debug
else
    exec "$@"
fi
