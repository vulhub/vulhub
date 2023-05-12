from django.contrib.gis.db import models

# Create your models here.

class Names(models.Model):
    name = models.CharField(max_length=128)

    def __str__(self):
        return self.name

class Collection(Names):
    path = models.LineStringField()

class Collection2(Names):
    point = models.PointField()