#!/bin/bash

/bin/wait-for-it.sh db:5432 -t 60 -- echo "postgres is up..."

python app.py migrate && python app.py runserver 0.0.0.0:8000