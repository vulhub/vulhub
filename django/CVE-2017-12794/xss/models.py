from django.db import models


class User(models.Model):
    username = models.CharField('Username', unique=True, max_length=32)
