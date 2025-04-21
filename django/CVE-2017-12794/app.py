#!/usr/bin/env python3
import os
os.environ.setdefault("DJANGO_SETTINGS_MODULE", __name__)
import sys
import dj_database_url
from django.conf.urls import url

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DEBUG = True
SECRET_KEY = '__secret_key__'
ALLOWED_HOSTS = ['*']

ROOT_URLCONF = __name__
TEMPLATES = [{
    'BACKEND': 'django.template.backends.django.DjangoTemplates',
    'DIRS': [os.path.join(BASE_DIR, 'templates')],
}]
DATABASES = {
    'default': dj_database_url.config(default='sqlite:///%s' % os.path.join(BASE_DIR, 'db.sqlite3'))
}
INSTALLED_APPS = [
   'xss'
]


def user(request):
    from django.http import HttpResponse
    from xss.models import User
    User.objects.create(username=request.GET['username'])
    return HttpResponse('Hello, user has been created!')


urlpatterns = [
    url(r'^create_user/$', user)
]


from django.core.management import execute_from_command_line
execute_from_command_line(sys.argv)
