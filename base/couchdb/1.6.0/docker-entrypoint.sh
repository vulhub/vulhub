#!/bin/bash
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.

set -e

if [ "$1" = 'couchdb' ]; then
    chown -R couchdb:couchdb \
		/usr/var/lib/couchdb \
		/usr/var/log/couchdb \
		/usr/var/run/couchdb \
		/usr/etc/couchdb

	chmod -R 0770 \
		/usr/var/lib/couchdb \
		/usr/var/log/couchdb \
		/usr/var/run/couchdb \
		/usr/etc/couchdb

	chmod 664 /usr/etc/couchdb/*.ini
	chmod 775 /usr/etc/couchdb/*.d

	if [ "$COUCHDB_USER" ] && [ "$COUCHDB_PASSWORD" ]; then
		# Create admin
		printf "[admins]\n%s = %s\n" "$COUCHDB_USER" "$COUCHDB_PASSWORD" > /usr/etc/couchdb/local.d/docker.ini
        chown couchdb:couchdb /usr/etc/couchdb/local.d/docker.ini
	fi

	printf "[httpd]\nport = %s\nbind_address = %s\n" ${COUCHDB_HTTP_PORT:=5984} ${COUCHDB_HTTP_BIND_ADDRESS:=0.0.0.0} > /usr/etc/couchdb/local.d/bind_address.ini
    chown couchdb:couchdb /usr/etc/couchdb/local.d/bind_address.ini

    exec gosu couchdb "$@"
fi

exec "$@"