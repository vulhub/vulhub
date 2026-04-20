#!/bin/bash
set -e

export LINKIS_HOME=/opt/linkis
export LINKIS_CONF_DIR=$LINKIS_HOME/conf

cd /opt/linkis-bin

echo "Waiting for MySQL at mysql:3306..."
until mysql -hmysql -P3306 -uroot -proot -e "SELECT 1" >/dev/null 2>&1; do
    sleep 2
done
echo "MySQL is ready."

if [ ! -f /opt/linkis/.installed ]; then
    echo "First-time installation - running install.sh"
    echo "2" | bash bin/install.sh
    sed -i 's|characterEncoding=UTF-8|characterEncoding=UTF-8\&useSSL=false|' /opt/linkis/conf/linkis.properties
    touch /opt/linkis/.installed
    echo "Installation complete."
fi

echo "Starting Linkis services..."
cd /opt/linkis
sh sbin/linkis-start-all.sh || true

echo "Linkis services started. Tailing logs..."
exec tail -F logs/*.log /dev/null
