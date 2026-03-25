#!/bin/bash

wait-for-it.sh db:1521 -t 0 -- echo "oracle is up"

until python app.py migrate; do
    echo "migrate failed, retrying in 5s..."
    sleep 5
done

python app.py makemigrations
python app.py migrate
python app.py loaddata collection.json
python app.py shell -c "from django.contrib.auth.models import User; User.objects.create_superuser('admin', 'admin@vulhub.org', 'admin') if not User.objects.filter(username='admin').exists() else 0"

exec python app.py runserver 0.0.0.0:8000
