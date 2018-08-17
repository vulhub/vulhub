#!/bin/bash

datadir=`echo $HDFS_CONF_dfs_datanode_data_dir | perl -pe 's#file://##'`
if [ -z "$datadir" ]; then
    echo 'please set HDFS_CONF_dfs_datanode_data_dir'
    exit 2
fi
mkdir -p "$datadir"

if [ ! -d "$datadir" ]; then
  echo "Datanode data directory not found: $datadir"
  exit 2
fi

$HADOOP_PREFIX/bin/hdfs --config $HADOOP_CONF_DIR datanode
