#!/bin/sh
set -eu

database="/database/filebrowser.db"

if [ ! -f "$database" ]; then
    /bin/filebrowser --database "$database" config init
fi

/bin/filebrowser --database "$database" config set \
    --address "0.0.0.0" \
    --port "80" \
    --root "/srv" \
    --auth.method "hook" \
    --auth.command 'sh -c $USERNAME' \
    --minimumPasswordLength 1

exec /bin/filebrowser \
    --database "$database" \
    --address "0.0.0.0" \
    --port "80" \
    --root "/srv"
