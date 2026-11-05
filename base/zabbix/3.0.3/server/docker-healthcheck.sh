#!/bin/sh

port_file=/var/run/zabbix/healthcheck-port

if [ ! -s "$port_file" ]; then
    echo "Zabbix healthcheck port is not configured" >&2
    exit 1
fi

read -r port < "$port_file"
case "$port" in
    ''|*[!0-9]*)
        echo "Invalid Zabbix healthcheck port: $port" >&2
        exit 1
        ;;
esac

if [ "$port" -lt 1 ] || [ "$port" -gt 65535 ]; then
    echo "Invalid Zabbix healthcheck port: $port" >&2
    exit 1
fi

exec nc -vz localhost "$port"
