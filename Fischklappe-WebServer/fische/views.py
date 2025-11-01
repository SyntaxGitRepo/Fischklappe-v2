# myapp/views.py
from rest_framework.decorators import api_view
from rest_framework.response import Response
from .models import ESPData
from django.http import HttpResponse
from django.template import loader
import socket, threading, django, os, asyncio
from asgiref.sync import async_to_sync
from channels.layers import get_channel_layer
@api_view(['POST'])
def receive_esp_data(request):
    """
    Receives a message from the ESP8266 and stores it.
    Expected JSON payload: {"id": id}
    """
    fisch_id = int(request.data.get("id"))
    if not fisch_id:
        return Response({"status": "error", "error": "No message provided"}, status=400)

    entry = ESPData.objects.create(fisch_id=fisch_id)

    channel_layer = get_channel_layer()
    async_to_sync(channel_layer.group_send)(
        "esp_updates",
        {
            "type": "esp_message",
            "data": {"action": "redirect", "target": f"/fische/{entry.fisch_id}/"}
        }
    )

    return Response({"status": "ok"})

def start_page(request):
    template = loader.get_template('index.html')
    return HttpResponse(template.render())

def fisch(request, fisch_id):
    template = loader.get_template('fische/' + str(fisch_id) + '.html')
    return HttpResponse(template.render())