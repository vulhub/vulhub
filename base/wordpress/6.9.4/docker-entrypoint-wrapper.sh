#!/bin/bash
set -euo pipefail

# Install WordPress automatically on first boot so the lab is ready to use
# without going through the setup wizard. The upstream entrypoint (invoked at
# the end of this script) copies the WordPress files and generates wp-config.php
# from the WORDPRESS_* environment variables first; meanwhile this background
# job waits for that config plus the database and then installs WordPress once.
(
	while [ ! -f /var/www/html/wp-config.php ]; do
		sleep 1
	done
	until php /usr/local/bin/auto-install.php; do
		sleep 2
	done
) &

exec docker-entrypoint.sh "$@"
