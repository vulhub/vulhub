#!/bin/bash

set -ex

service cron start && rsync --no-detach --daemon --config /etc/rsyncd.conf
