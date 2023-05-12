#!/bin/bash

set -ex
cd /usr/src

wait-for-it.sh -t 0 db:5432 -- echo "postgres is up"

python manage.py migrate
python manage.py loaddata collection.json
python manage.py shell -c "from django.contrib.auth.models import User; User.objects.create_superuser('admin', 'admin@vulhub.org', 'a123123123') if not User.objects.filter(username='admin').exists() else 0"

exec "$@"