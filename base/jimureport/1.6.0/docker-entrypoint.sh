#!/bin/bash

set -ex

echo "Waiting for database to start..."
while ! mysqladmin ping -h "${MYSQL_HOST:-127.0.0.1}" -P "${MYSQL_PORT:-3306}" -u"${MYSQL_USERNAME:-root}" -p"${MYSQL_PASSWORD:-root}" --silent; do
    sleep 1
done

if ! mysql -h "${MYSQL_HOST:-127.0.0.1}" -P "${MYSQL_PORT:-3306}" -u"${MYSQL_USERNAME:-root}" -p"${MYSQL_PASSWORD:-root}" -D "${MYSQL_DB:-jimureport}" -e 'describe jimu_dict;' > /dev/null 2>&1; then
    echo "database does not exist, initializing..."
    mysql -h "${MYSQL_HOST:-127.0.0.1}" -P "${MYSQL_PORT:-3306}" -u"${MYSQL_USERNAME:-root}" -p"${MYSQL_PASSWORD:-root}" -D "${MYSQL_DB:-jimureport}" < /opt/jimureport/jimureport.mysql5.7.create.sql
else
    echo "database have already initialized, skip..."
fi

exec "$@"
