#!/usr/bin/env sh

PRG="$0"
PRGDIR=`dirname "$PRG"`
[ -z "$WEBAPP_HOME" ] && WEBAPP_HOME=`cd "$PRGDIR/.." >/dev/null; pwd`

JAVA_OPTS=" -Xms256M -Xmx512M"
JAR_PATH="${WEBAPP_HOME}/webapp"

exec java ${JAVA_OPTS} -jar ${JAR_PATH}/skywalking-webapp.jar \
         --spring.config.location=${JAR_PATH}/webapp.yml \
         --logging.file=/dev/stderr