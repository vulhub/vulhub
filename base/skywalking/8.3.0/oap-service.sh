#!/usr/bin/env sh

PRG="$0"
PRGDIR=`dirname "$PRG"`
[ -z "$OAP_HOME" ] && OAP_HOME=`cd "$PRGDIR/.." >/dev/null; pwd`

OAP_LOG_DIR="${OAP_LOG_DIR:-${OAP_HOME}/logs}"
JAVA_OPTS=" -Xms256M -Xmx512M"

if [ ! -d "${OAP_LOG_DIR}" ]; then
    mkdir -p "${OAP_LOG_DIR}"
fi

CLASSPATH="$OAP_HOME/config:$CLASSPATH"
for i in "$OAP_HOME"/oap-libs/*.jar
do
    CLASSPATH="$i:$CLASSPATH"
done

exec java ${JAVA_OPTS} -Doap.logDir=${OAP_LOG_DIR} -classpath $CLASSPATH org.apache.skywalking.oap.server.starter.OAPServerStartUp
