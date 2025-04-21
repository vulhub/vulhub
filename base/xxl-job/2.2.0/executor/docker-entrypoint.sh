#!/bin/bash

set -ex

wait-for-it -t 0 admin:8080

exec "$@"
