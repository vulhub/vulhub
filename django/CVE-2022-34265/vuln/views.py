from django.http import HttpResponse, JsonResponse
from django.db.models.functions import Trunc
from django.db.models import Count
from .models import WebLog


def create_log(request):
    method = request.method
    url = request.build_absolute_uri()
    user_agent = request.META.get('HTTP_USER_AGENT')
    WebLog.objects.create(
        method=method,
        url=url,
        user_agent=user_agent
    )


def vul(request):
    create_log(request)
    date = request.GET.get('date', 'minute')
    objects = list(WebLog.objects.annotate(time=Trunc('created_time', date)).values('time').order_by('-time').annotate(count=Count('id')))
    return JsonResponse(data=objects, safe=False)
