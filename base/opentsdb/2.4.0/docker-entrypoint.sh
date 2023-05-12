#!/bin/bash

set -e

if [[ "$1" == "" ]]; then
    /opt/hbase/bin/start-hbase.sh

    cd /usr/share/opentsdb
    COMPRESSION=NONE HBASE_HOME=/opt/hbase ./tools/create_table.sh
    # Creating some dummy metrics to test against
    ./bin/tsdb mkmetric sys.cpu.user sys.cpu.nice sys.cpu.idle
    ./bin/tsdb tsd --config /etc/opentsdb/opentsdb.conf
fi

exec "$@"
