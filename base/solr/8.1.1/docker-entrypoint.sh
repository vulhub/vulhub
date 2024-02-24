#!/bin/bash

set -euo pipefail

CORE="demo"
coresdir=/opt/solr/server/solr
CORE_DIR="$coresdir/$CORE"

if [ -d "$CORE_DIR" ]; then
  echo "$CORE_DIR exists; skipping core creation"
else
  # start Solr background
  SOLR_OPTS="-Djetty.host=localhost" solr start -force -p 8983
  # wait for Solr startup
  while ! wget -qO- http://localhost:8983 | grep -i solr >/dev/null; do
    echo "waiting for Apache Solr ready..."
    sleep 1
  done
  

  echo "Creating $CORE"
  solr create_core -force -c "$CORE" -d example/example-DIH/solr/db
  echo "Created $CORE"
  echo "Loading example data"

  post -c $CORE -commit no example/exampledocs/*.xml
  post -c $CORE -commit no example/exampledocs/books.json
  post -c $CORE -commit yes example/exampledocs/books.csv

  echo "Loaded example data"
  # check the core_dir exists; otherwise the detecting above will fail after stop/start
  if [ ! -d "$CORE_DIR" ]; then
    echo "Missing $CORE_DIR"
    exit 1
  fi

  # stop Solr background
  SOLR_OPTS="-Djetty.host=localhost" solr stop -force -p 8983
fi

exec "$@"
