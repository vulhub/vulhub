#!/bin/bash
set -e

wait-for-it -t 0 db:3306
if [[ $(mysql -hdb -uroot -proot -e "SHOW DATABASES LIKE 'xxl_job';") == "" ]]; then 
    mysql -hdb -uroot -proot < /usr/src/tables_xxl_job.sql
fi

exec "$@"