#!/bin/bash
# DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER.
# 
# Copyright (c) 2017 Oracle and/or its affiliates. All rights reserved.
set -ex

if [[ -z $ADMIN_PASSWORD ]]; then
	ADMIN_PASSWORD=$(date| md5sum | fold -w 8 | head -n 1)
	echo "##########GENERATED ADMIN PASSWORD: $ADMIN_PASSWORD  ##########"
fi

if [[ ! -e /glassfishpwd ]]; then
	echo "AS_ADMIN_PASSWORD=" > /glassfishpwd
	echo "AS_ADMIN_NEWPASSWORD=${ADMIN_PASSWORD}" >> /glassfishpwd
	asadmin --user=admin --passwordfile=/glassfishpwd --secure=false change-admin-password --domain_name domain1
	asadmin --secure=false start-domain
	echo "AS_ADMIN_PASSWORD=${ADMIN_PASSWORD}" > /glassfishpwd
	asadmin --user=admin --passwordfile=/glassfishpwd --secure=false enable-secure-admin
	asadmin --user=admin --passwordfile=/glassfishpwd --secure=false stop-domain
fi

exec "$@"