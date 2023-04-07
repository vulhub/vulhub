#!/bin/bash

set -ex

wait-for-it "${JOOMLA_DB_HOST:-localhost}:${JOOMLA_DB_PORT:-3306}" -- echo "database is available"

if [[ ! -e "/var/www/html/configuration.php" ]]; then
    cd /var/www/html || exit 1
    /root/.composer/vendor/bin/joomla site:install --www /var/www/html -L "${JOOMLA_DB_USER:-root}:${JOOMLA_DB_PASSWORD:-root}" -H "${JOOMLA_DB_HOST:-localhost}" -P "${JOOMLA_DB_PORT:-3306}" --mysql-db-prefix joomla_ --mysql-database "${JOOMLA_DB_NAME:-joomla}" --mysql-driver mysqli -- ./
fi

exec "$@"
