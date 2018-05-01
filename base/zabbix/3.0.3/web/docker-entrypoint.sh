#!/bin/sh

__stop() {
 echo "> SIGTERM signal received, try to gracefully shutdown all services..."
 echo "> Stopping: nginx"
 kill $(cat /var/run/nginx.pid)
 tail -n50 /var/log/nginx/error.log
 echo "> Stopping: php-fpm"
 kill $(cat /var/run/php-fpm.pid)
}

trap "__stop; exit 0" SIGTERM SIGINT


# frontend config
sed -i "s/^\(\$DB\['TYPE'\]\).*/\1 = 'MYSQL';/" /usr/share/webapps/zabbix/conf/zabbix.conf.php
sed -i "s/^\(\$DB\['SERVER'\]\).*/\1 = '$DATABASE_HOST';/" /usr/share/webapps/zabbix/conf/zabbix.conf.php
sed -i "s/^\(\$DB\['PORT'\]\).*/\1 = '$DATABASE_PORT';/" /usr/share/webapps/zabbix/conf/zabbix.conf.php
sed -i "s/^\(\$DB\['DATABASE'\]\).*/\1 = '$DATABASE_NAME';/" /usr/share/webapps/zabbix/conf/zabbix.conf.php
sed -i "s/^\(\$DB\['USER'\]\).*/\1 = '$DATABASE_USER';/" /usr/share/webapps/zabbix/conf/zabbix.conf.php
sed -i "s/^\(\$DB\['PASSWORD'\]\).*/\1 = '$DATABASE_PASS';/" /usr/share/webapps/zabbix/conf/zabbix.conf.php

sed -i "s/^\(\$ZBX_SERVER\)\s.*/\1 = '$ZBX_SRV_HOST';/" /usr/share/webapps/zabbix/conf/zabbix.conf.php
sed -i "s/^\(\$ZBX_SERVER_PORT\)\s.*/\1 = '$ZBX_SRV_PORT';/" /usr/share/webapps/zabbix/conf/zabbix.conf.php
sed -i "s/^\(\$ZBX_SERVER_NAME\)\s.*/\1 = '$ZBX_SRV_NAME';/" /usr/share/webapps/zabbix/conf/zabbix.conf.php

echo "> Starting: php-fpm"
php-fpm

echo "> Starting: nginx"
/usr/sbin/nginx
tail -n100 /var/log/nginx/error.log

while true; do
 sleep 15s & wait $!;
 ps -o pid |egrep "^\s+$(cat /var/run/nginx.pid)\$" 1>/dev/null || exit 1;
 ps -o pid |egrep "^\s+$(cat /var/run/php-fpm.pid)\$" 1>/dev/null || exit 1;
done