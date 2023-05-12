#!/bin/sh

cron -L15

rsyslogd

/bin/sh -c "/opt/activemq/bin/activemq console"