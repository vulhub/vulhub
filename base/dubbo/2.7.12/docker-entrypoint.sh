#!/bin/sh

set -e

case "$1" in
    provider)
        shift
        exec java $JAVA_OPTS -cp /dubbo-sample-1.0-SNAPSHOT.jar org.vulhub.Provider "$@"
        ;;
    consumer)
        shift
        exec java -agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005 $JAVA_OPTS -cp /dubbo-sample-1.0-SNAPSHOT.jar org.vulhub.Consumer "$@"
        ;;
    *)
        exec java -agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005 $JAVA_OPTS -cp /dubbo-sample-1.0-SNAPSHOT.jar org.vulhub.Consumer "$@"
        ;;
esac
