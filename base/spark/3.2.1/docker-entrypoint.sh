#!/usr/bin/env bash

set -e

if [ "$#" -lt 1 ]; then
    set -- master
fi

if [ "$1" = "master" ]; then
    cat > "${SPARK_HOME}/conf/spark-defaults.conf" <<'EOF'
spark.acls.enable true
spark.ui.view.acls.groups spark
EOF
    exec start-master.sh "${@:2}"
fi

exec "$@"
