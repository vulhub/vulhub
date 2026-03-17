#!/bin/bash

set -e

DB_PROPS="/data/teamcity_server/datadir/config/database.properties"
ADMIN_MARKER="/data/teamcity_server/datadir/.admin_created"

if [ ! -f "$DB_PROPS" ]; then
    mkdir -p /data/teamcity_server/datadir/config
    echo 'connectionUrl=jdbc:hsqldb:file:$TEAMCITY_SYSTEM_PATH/buildserver' > "$DB_PROPS"
fi

if [ ! -f "$ADMIN_MARKER" ]; then
    (
        # Wait for TeamCity to be ready
        until curl -sf -o /dev/null http://localhost:8111/showAgreement.html; do
            sleep 5
        done

        # Accept license agreement
        CSRF_TOKEN=$(curl -s http://localhost:8111/showAgreement.html | grep -o 'value="[a-f0-9-]*"' | tail -1 | cut -d'"' -f2)
        curl -s -o /dev/null -X POST http://localhost:8111/showAgreement.html \
            -d "accept=true&_accept=&_sendUsageStatistics=&Continue=Continue&tc-csrf-token=$CSRF_TOKEN"

        # Read super user token from log
        TOKEN=""
        while [ -z "$TOKEN" ]; do
            TOKEN=$(grep -o 'Super user authentication token: [0-9]*' /opt/teamcity/logs/teamcity-server.log 2>/dev/null | tail -1 | awk '{print $NF}')
            sleep 1
        done

        # Create admin user
        curl -s -o /dev/null -X POST "http://localhost:8111/app/rest/users" \
            -u ":$TOKEN" \
            -H "Content-Type: application/json" \
            -d '{"username":"admin","password":"admin","roles":{"role":[{"roleId":"SYSTEM_ADMIN","scope":"g"}]}}'

        touch "$ADMIN_MARKER"
        echo "Admin user created: admin/admin"
    ) &
fi

exec "$@"
