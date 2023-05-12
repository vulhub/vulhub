#!/bin/bash

COUCHDB_URL=${COUCHDB_URL:-"couchdb:5984"}
COUCHDB_AUTH=${COUCHDB_AUTH:-"admin:password"}

while ! curl -m 5 "${COUCHDB_URL}"; do
    sleep 1
done

curl -X PUT http://${COUCHDB_AUTH}@${COUCHDB_URL}/_users
curl -X PUT http://${COUCHDB_AUTH}@${COUCHDB_URL}/_replicator
curl -X PUT http://${COUCHDB_AUTH}@${COUCHDB_URL}/_global_changes
