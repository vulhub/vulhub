#!/bin/bash
set -e

wait-for-it -t 0 jeecg-boot-mysql:3306
if [[ $(mysql -h jeecg-boot-mysql -uroot -proot -e "SHOW DATABASES LIKE 'jeecg-boot';") == "" ]]; then
    mysql -h jeecg-boot-mysql -uroot -proot < /usr/src/jeecgboot-mysql-5.7.sql
fi

exec "$@"