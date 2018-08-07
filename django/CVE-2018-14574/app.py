import os
import sys
from django.urls import include, path, re_path
from django.http import HttpResponse

os.environ.setdefault("DJANGO_SETTINGS_MODULE", __name__)
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DEBUG = False
SECRET_KEY = 'vulhub'

ALLOWED_HOSTS = ['*']
MIDDLEWARE = [
    'django.middleware.common.CommonMiddleware',
]

ROOT_URLCONF = __name__
TEMPLATES = [{
    'BACKEND': 'django.template.backends.django.DjangoTemplates',
    'DIRS': [os.path.join(BASE_DIR, 'templates')]
}]
LOGGING = {
    'version': 1,
    'disable_existing_loggers': False,
    'handlers': {
        'console': {
            'class': 'logging.StreamHandler',
        },
    },
    'loggers': {
        'django': {
            'handlers': ['console'],
            'level': os.getenv('DJANGO_LOG_LEVEL', 'WARNING'),
        },
    },
}


def home(request, path=None):
    return HttpResponse('Hello, world.')


urlpatterns = [
    path('', home),
    re_path(r'^(.*)/$', home),
]

from django.core.management import execute_from_command_line
execute_from_command_line(sys.argv)
