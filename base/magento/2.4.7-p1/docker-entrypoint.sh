#!/bin/bash

set -ex

# Check if the Magento directory exists
cd /var/www/html || exit 1

wait-for-it "$DB_HOST" --timeout=0 --strict -- echo "MySQL is up"
wait-for-it "$ELASTICSEARCH_HOST:$ELASTICSEARCH_PORT" --timeout=0 --strict -- echo "ElasticSearch is up"

if [[ ! -e app/etc/config.php ]]; then
    bin/magento setup:install \
        --base-url=http://localhost:8080/ \
        --backend-frontname=admin \
        --db-host="$DB_HOST" \
        --db-name="$DB_NAME" \
        --db-user="$DB_USER" \
        --db-password="$DB_PASSWORD" \
        --admin-firstname=admin \
        --admin-lastname=admin \
        --admin-email="$ADMIN_EMAIL" \
        --admin-user="$ADMIN_USER" \
        --admin-password="$ADMIN_PASSWORD" \
        --search-engine=elasticsearch7 \
        --elasticsearch-host="$ELASTICSEARCH_HOST" \
        --elasticsearch-port="$ELASTICSEARCH_PORT" \
        --language=en_US \
        --currency=USD \
        --timezone=Asia/Singapore \
        --use-rewrites=1
    bin/magento setup:di:compile
    bin/magento setup:static-content:Deploy -f
    chown www-data:www-data -R pub/ var/ generated/
fi

# Execute the original entrypoint with the same arguments
exec docker-php-entrypoint "$@"
