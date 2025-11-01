# myapp/consumers.py
from channels.generic.websocket import AsyncWebsocketConsumer
import json

class ESPConsumer(AsyncWebsocketConsumer):
    async def connect(self):
        await self.channel_layer.group_add("esp_updates", self.channel_name)
        await self.accept()

    async def disconnect(self, close_code):
        await self.channel_layer.group_discard("esp_updates", self.channel_name)

    async def esp_message(self, event):
        # Forward message to WebSocket client
        await self.send(text_data=json.dumps(event["data"]))
