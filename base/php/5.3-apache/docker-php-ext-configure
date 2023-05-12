#!/bin/bash
set -e

ext="$1"
extDir="/usr/src/php/ext/$ext"
if [ -z "$ext" -o ! -d "$extDir" ]; then
    echo >&2 "usage: $0 ext-name [configure flags]"
    echo >&2 "   ie: $0 gd --with-jpeg-dir=/usr/local/something"
    echo >&2
    echo >&2 'Possible values for ext-name:'
    echo >&2 $(find /usr/src/php/ext -mindepth 2 -maxdepth 2 -type f -name 'config.m4' | cut -d/ -f6 | sort)
    exit 1
fi
shift

set -x
cd "$extDir"
phpize
./configure "$@"