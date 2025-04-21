#!/bin/sh

# You could probably do this fancier and have an array of extensions
# to create, but this is mostly an illustration of what can be done

psql -v ON_ERROR_STOP=1 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" <<EOF
    create extension pg_trgm;
    select * FROM pg_extension;
EOF
