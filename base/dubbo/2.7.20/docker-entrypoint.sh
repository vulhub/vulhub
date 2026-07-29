#!/bin/sh

exec java -agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005 $JAVA_OPTS -jar /dubbo-sample-1.0-SNAPSHOT.jar
