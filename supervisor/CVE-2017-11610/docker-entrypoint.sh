#!/bin/bash

supervisord --user nobody -c /usr/local/etc/supervisord.conf

while true; do sleep 30; done;