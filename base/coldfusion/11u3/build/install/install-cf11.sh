#!/bin/sh
#
# Script based on https://forums.adobe.com/message/4727551

cd /tmp
if [ ! -f "ColdFusion_11_WWEJ_linux64.bin" ]; then
        wget https://file.vulhub.org/download/coldfusion/ColdFusion_11_WWEJ_linux64.bin
        chmod 755 ColdFusion_11_WWEJ_linux64.bin
fi

useradd -c "user for colfusion" -M -G www-data coldfusion

/tmp/ColdFusion_11_WWEJ_linux64.bin -f installer.profile

if [ -f "ColdFusion_11_WWEJ_linux64.bin" ]; then
  rm ColdFusion_11_WWEJ_linux64.bin
fi
