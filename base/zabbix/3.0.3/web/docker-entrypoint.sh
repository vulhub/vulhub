#!/bin/sh

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

exec /usr/sbin/httpd -D FOREGROUND