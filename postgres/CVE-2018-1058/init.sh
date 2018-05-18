#!/bin/bash

set -ex 

psql -v ON_ERROR_STOP=1 --username "$POSTGRES_USER" <<-EOSQL
    CREATE USER "vulhub" WITH PASSWORD 'vulhub';
    CREATE DATABASE "vulhub" OWNER "vulhub";
    GRANT ALL PRIVILEGES ON DATABASE "vulhub" to "vulhub";
EOSQL

psql -v ON_ERROR_STOP=1  --username "$POSTGRES_USER" vulhub <<-EOSQL
    CREATE EXTENSION IF NOT EXISTS dblink;
EOSQL