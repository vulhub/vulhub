#!/bin/bash

set -ex

wait-for-it admin:8080

exec "$@"
