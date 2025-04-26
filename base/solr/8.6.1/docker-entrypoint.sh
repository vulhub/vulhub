#!/bin/bash

set -euo pipefail

CORE=demo
coresdir=/opt/solr/server/solr
CORE_DIR="$coresdir/demo"

if [ -d "$CORE_DIR" ]; then
  echo "$CORE_DIR exists; skipping demo creation"
else
  # start Solr background
  solr start -cloud -force -p 8983 -s /opt/solr/example/cloud
  # wait for Solr startup
  while ! wget -qO- http://localhost:8983 | grep -i solr >/dev/null; do
    echo "waiting for Apache Solr ready..."
    sleep 1
  done
  
  echo "Creating $CORE"
  solr create -force -c "$CORE"
  echo "Created $CORE"
  echo "Loading example data"

  post -c $CORE -commit no example/exampledocs/*.xml
  post -c $CORE -commit no example/exampledocs/books.json
  post -c $CORE -commit yes example/exampledocs/books.csv
  echo "Loaded example data"
  # check the core_dir exists; otherwise the detecting above will fail after stop/start
  if ! wget -qO- "http://localhost:8983/solr/admin/collections?action=list&wt=json" | grep -i "$CORE" >/dev/null; then
    echo "Missing $CORE_DIR"
    exit 1
  fi

  # stop Solr background
  solr stop -force -p 8983
  mkdir "$CORE_DIR"
fi

exec "$@"
