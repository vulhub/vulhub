#!/bin/bash

deployUser=hadoop
deployPwd=hadoop

LINKIS_SERVER_VERSION=v1

WORKSPACE_USER_ROOT_PATH=file:///tmp/linkis/
HDFS_USER_ROOT_PATH=file:///tmp/linkis/hdfs
ENGINECONN_ROOT_PATH=/tmp/linkis/engineconn
RESULT_SET_ROOT_PATH=file:///tmp/linkis/result

#YARN_RESTFUL_URL="http://127.0.0.1:8088"

#HADOOP_HOME=/appcom/Install/hadoop
#HADOOP_CONF_DIR=/appcom/config/hadoop-config
#HIVE_HOME=/appcom/Install/hive
#HIVE_CONF_DIR=/appcom/config/hive-config
#SPARK_HOME=/appcom/Install/spark
#SPARK_CONF_DIR=/appcom/config/spark-config

EUREKA_PORT=20303
export EUREKA_PREFER_IP=false

GATEWAY_PORT=9001
MANAGER_PORT=9101
ENGINECONNMANAGER_PORT=9102
ENGINECONN_PLUGIN_SERVER_PORT=9103
ENTRANCE_PORT=9104
PUBLICSERVICE_PORT=9105

export SERVER_HEAP_SIZE="256M"

LINKIS_VERSION=1.3.0
LINKIS_PUBLIC_MODULE=lib/linkis-commons/public-module

export PROMETHEUS_ENABLE=false
export ENABLE_HDFS=false
export ENABLE_HIVE=false
export ENABLE_SPARK=false
