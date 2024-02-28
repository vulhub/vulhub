#!/bin/bash

set -e

if [[ "$1" == "" ]]; then
    /opt/hbase/bin/start-hbase.sh

    cd /usr/share/opentsdb
    COMPRESSION=NONE HBASE_HOME=/opt/hbase ./tools/create_table.sh
    ./bin/tsdb tsd --config /etc/opentsdb/opentsdb.conf
fi

exec "$@"