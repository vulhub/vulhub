#!/bin/bash

set -ex

cd /usr/share/elasticsearch/bin && ./elasticsearch -d

exec "$@"