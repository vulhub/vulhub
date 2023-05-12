#!/bin/sh

__stop() {
 echo "> SIGTERM signal received, try to gracefully shutdown all services..."
 echo "> Stopping: Agent"
 kill $(cat /var/run/zabbix/zabbix_agentd.pid)
 tail -n50 /var/log/zabbix/zabbix_agentd.log
 echo "> Stopping: Server"
 kill $(cat /var/run/zabbix/zabbix_server.pid)
 tail -n50 /var/log/zabbix/zabbix_server.log
}

trap "__stop; exit 0" SIGTERM SIGINT

if [ "$1" == "server" ]; then
    sed -e "s/.*\(ListenPort=\).*/\1$ZBX_SRV_PORT/g" \
        -e "s/.*\(DBHost=\).*/\1$DATABASE_HOST/g" \
        -e "s/^\(DBName=\).*/\1$DATABASE_NAME/g" \
        -e "s/.*\(DBPort=\).*/\1$DATABASE_PORT/g" \
        -e "s/^\(DBUser=\).*/\1$DATABASE_USER/g" \
        -e "s/.*\(DBPassword=\).*/\1$DATABASE_PASS/g" -i /etc/zabbix/zabbix_server.conf
    echo "> Running zabbix server..."
    exec su zabbix -s "/bin/bash" -c "/usr/sbin/zabbix_server --foreground -c /etc/zabbix/zabbix_server.conf"
elif [ "$1" == "agent" ]; then
    sed -e "s/^\(Server=\).*/\1$ZBX_SRV_HOST/g" \
        -e "s/^\(ServerActive=\).*/\1$ZBX_SRV_HOST_ACT/g" \
        -e "s/.*\(StartAgents=\).*/\1$ZBX_AGT_NUM/g" \
        -e "s/.*\(ListenPort=\).*/\1$ZBX_AGT_PORT/g" \
        -e "s/^\(Hostname=\).*/\1`hostname`/g" -i /etc/zabbix/zabbix_agentd.conf
    echo "> Running zabbix server..."
    exec su zabbix -s "/bin/bash" -c "/usr/sbin/zabbix_agentd --foreground -c /etc/zabbix/zabbix_agentd.conf"
else
    exec "$@"
fi