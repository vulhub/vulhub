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

	exec gosu couchdb "$@"
fi

exec "$@"