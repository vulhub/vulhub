#!/bin/bash

until ([ "$health_check" = 'yellow' ] || [ "$health_check" = 'green' ]); do
    health_check="$(curl -fsSL "http://${UNOMI_ELASTICSEARCH_ADDRESSES}/_cat/health?h=status")"
    >&2 echo "Elastic Search is unavailable - waiting"
    sleep 1
done

exec "$@"
