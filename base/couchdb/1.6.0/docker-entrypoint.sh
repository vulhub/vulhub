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
	# we need to set the permissions here because docker mounts volumes as root
	chown -R couchdb:couchdb \
		/usr/local/var/lib/couchdb \
		/usr/local/var/log/couchdb \
		/usr/local/var/run/couchdb \
		/usr/local/etc/couchdb

	chmod -R 0770 \
		/usr/local/var/lib/couchdb \
		/usr/local/var/log/couchdb \
		/usr/local/var/run/couchdb \
		/usr/local/etc/couchdb

	chmod 664 /usr/local/etc/couchdb/*.ini
	chmod 775 /usr/local/etc/couchdb/*.d

	if [ "$COUCHDB_USER" ] && [ "$COUCHDB_PASSWORD" ]; then
		# Create admin
		printf "[admins]\n%s = %s\n" "$COUCHDB_USER" "$COUCHDB_PASSWORD" > /usr/local/etc/couchdb/local.d/docker.ini
		chown couchdb:couchdb /usr/local/etc/couchdb/local.d/docker.ini
	fi

	printf "[httpd]\nport = %s\nbind_address = %s\n" ${COUCHDB_HTTP_PORT:=5984} ${COUCHDB_HTTP_BIND_ADDRESS:=0.0.0.0} > /usr/local/etc/couchdb/local.d/bind_address.ini
	chown couchdb:couchdb /usr/local/etc/couchdb/local.d/bind_address.ini

	# if we don't find an [admins] section followed by a non-comment, display a warning
	if ! grep -Pzoqr '\[admins\]\n[^;]\w+' /usr/local/etc/couchdb; then
		# The - option suppresses leading tabs but *not* spaces. :)
		cat >&2 <<-'EOWARN'
			****************************************************
			WARNING: CouchDB is running in Admin Party mode.
			         This will allow anyone with access to the
			         CouchDB port to access your database. In
			         Docker's default configuration, this is
			         effectively any other container on the same
			         system.
			         Use "-e COUCHDB_USER=admin -e COUCHDB_PASSWORD=password"
			         to set it in "docker run".
			****************************************************
		EOWARN
	fi

	exec gosu couchdb "$@"
fi

exec "$@"