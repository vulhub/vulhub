#!/bin/bash

wait-for-it.sh db:5432 -t 0 -- echo "database is up"

python app.py migrate

exec python app.py runserver 0.0.0.0:8000
