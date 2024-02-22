#!/bin/bash

set -e

wait-for-it -t 0 "${MYSQL_SERVER}" -- echo "MySQL is up"
wait-for-it -t 0 "${KAFKA_SERVER}" -- echo "Kafka is up"

exec "$@"
