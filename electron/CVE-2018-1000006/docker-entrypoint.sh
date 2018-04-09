#!/bin/bash

set -ex 

cd /project && npm install && npm run build-${ARCH:=64}

if [ -d "/build/vulhub-app-win32-x64" ] || [ -d "/build/vulhub-app-win32-ia32" ]; then
    tar -zcvf /build/vulhub-app.tar.gz /build/vulhub-app-win32-*
    rm -rf /build/vulhub-app-win32-*
fi