#!/bin/bash

set -ex 

cd /project && npm install && npm run build-${PLATFORM:=win64}

tar -zcvf /build/cve-2018-15685.tar.gz /build/cve-2018-15685-* --exclude /build/cve-2018-15685.tar.gz
rm -rf /build/cve-2018-15685-*