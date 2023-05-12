#!/bin/bash

wait-for-it db:3306 -- echo "database is up"

if [[ ! -e ./db/local.php ]]; then
    php console.php database:configure --host db -- root root tikiwiki
    php console.php database:install
    php console.php users:password -- admin vulhub
    php console.php index:rebuild
    php console.php installer:lock
fi

apache2-foreground
