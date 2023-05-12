#!/bin/bash

set -ex

cd /opt/
if [ ! -f "coldfusion-801-lin64.bin" ]; then
        wget -O coldfusion-801-lin64.bin https://vulhub.oss-cn-shanghai.aliyuncs.com/download/coldfusion/coldfusion-801-lin64.bin
        chmod 755 coldfusion-801-lin64.bin
fi

./coldfusion-801-lin64.bin -f installer.properties

if [ -f "coldfusion-801-lin64.bin" ]; then
  rm coldfusion-801-lin64.bin
fi