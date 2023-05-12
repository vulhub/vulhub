#!/bin/bash

set -ex

wait-for-it -t 0 "${JOOMLA_DB_HOST:-localhost}:${JOOMLA_DB_PORT:-3306}" -- echo "database is available"

if [[ ! -e "/var/www/html/configuration.php" ]]; then
    joomla site:install --www /var/www -L "${JOOMLA_DB_USER:-root}:${JOOMLA_DB_PASSWORD:-root}" -H "${JOOMLA_DB_HOST:-localhost}" -P "${JOOMLA_DB_PORT:-3306}" --mysql-database "${JOOMLA_DB_NAME:-joomla}" --mysql-driver mysqli --sample-data=blog -- html
    sed -i "s/public \$debug = 1;/public \$debug = 0;/" /var/www/html/configuration.php
    chown -R www-data:www-data /var/www/html
fi

exec "$@"
