#!/bin/bash

wait-for-it.sh db:3306 -t 0 -- echo "mysql is up"

python app.py migrate
python app.py loaddata collection.json

exec python app.py runserver 0.0.0.0:8000
