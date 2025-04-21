from django.shortcuts import render, HttpResponse
from django.contrib.gis.geos import Point
from django.contrib.gis.measure import D
from django.contrib.gis.db.models.functions import Distance
from django.contrib.gis.db.models import Union
from .models import Collection, Collection2

# Create your views here.

def vuln(request):
    query = request.GET.get('q', default=0.05)
    qs = Collection.objects.annotate(
        d=Distance(
            Point(0.01, 0.01, srid=4326),
            Point(0.01, 0.01, srid=4326),
            tolerance=query,
        ),
    ).filter(d=D(m=1)).values('name')
    return HttpResponse(qs)

def vuln2(request):
    query = request.GET.get('q')
    qs = Collection2.objects.aggregate(
            Union('point', tolerance=query),
    ).values()

    return HttpResponse(qs)