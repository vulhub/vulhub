#!/bin/bash

set -e

cd /usr/share/elasticsearch/bin && ./elasticsearch -d

exec "$@"