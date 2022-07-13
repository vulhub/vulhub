from operator import mod
from django.db import models


class WebLog(models.Model):
    method = models.CharField(max_length=16)
    url = models.CharField(max_length=256)
    user_agent = models.CharField(max_length=256)
    created_time = models.DateTimeField(auto_now_add=True)
