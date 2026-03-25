from django.db import models
from django.contrib.postgres.fields import JSONField


class Collection(models.Model):
    name = models.CharField(max_length=128)
    detail = JSONField()

    def __str__(self):
        return self.name
