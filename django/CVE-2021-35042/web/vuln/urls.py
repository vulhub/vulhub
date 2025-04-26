from django.urls import include, path, re_path
from . import views


urlpatterns = [
    path('vuln/', views.vul),
]
