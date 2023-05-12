#!/bin/bash
set -ex

wait-for-it db:3306 -t 300 -- echo "database is connected"
if [[ ! -e ".env" ]]; then
    echo -e "db\nv2board\nroot\nroot\nadmin@example.com\nexample123" | php artisan v2board:install
    sed -i 's/REDIS_HOST=.*/REDIS_HOST=redis/g' .env
fi

chown www-data:www-data -R /var/www/html
# first arg is `-f` or `--some-option`
if [ "${1#-}" != "$1" ]; then
	set -- apache2-foreground "$@"
fi

exec "$@"
