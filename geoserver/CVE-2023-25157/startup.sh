#!/bin/bash

AUTH="admin:geoserver"
/mnt/geoserver/bin/startup.sh &
GEOSERVER_PID=$!

SERVER_ALIVE=$(curl -s --output /dev/null -u "$AUTH" -XGET --write-out "%{http_code}" http://localhost:8080/geoserver/rest/about/status)
while [ "$SERVER_ALIVE" -ne 200 ]
do
  echo "geoserver is not ready yet, waiting for 1 second..."
  sleep 1
  SERVER_ALIVE=$(curl -s --output /dev/null -u "$AUTH" -XGET --write-out "%{http_code}" http://localhost:8080/geoserver/rest/about/status)
done

VULHUB_EXISTS=$(curl -s --output /dev/null -u "$AUTH" -XGET --write-out "%{http_code}" http://localhost:8080/geoserver/rest/workspaces/vulhub)
if [ "${VULHUB_EXISTS}" -eq 200 ]; then
    echo "vulhub wordspace already exists"
else
    # refer to <https://github.com/geoserver/geoserver-history/blob/master/doc/en/user/source/restconfig/rest-config-examples/rest-config-examples-curl.rst>
    curl -XPOST -u "$AUTH" -H "Content-type: application/json" -d '{"workspace":{"name":"vulhub"}}' http://localhost:8080/geoserver/rest/workspaces
    curl -u "$AUTH" -XPOST -H 'Content-Type: application/json' \
        -d '{"dataStore":{"name":"pg","connectionParameters":{"host":"postgres","port":5432,"database":"geoserver","user":"postgres","passwd":"vulhub","dbtype":"postgis","createDatabase":true}}}' \
        http://localhost:8080/geoserver/rest/workspaces/vulhub/datastores
    curl -u "$AUTH" -XPOST -H 'Content-Type: application/json' -d '{"featureType":{"name":"example","attributes":{"attribute":[{"name":"name","binding":"java.lang.String"}]}}}' http://localhost:8080/geoserver/rest/workspaces/vulhub/datastores/pg/featuretypes
    echo "target geoserver is initialized successfully"
fi

wait $GEOSERVER_PID
