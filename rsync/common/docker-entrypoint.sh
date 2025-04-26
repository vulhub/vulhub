#!/bin/bash

set -ex

service cron start 

exec rsync --no-detach --daemon --config /etc/rsyncd.conf
