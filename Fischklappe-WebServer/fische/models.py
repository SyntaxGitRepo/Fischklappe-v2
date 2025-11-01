# myapp/models.py
from django.db import models

class ESPData(models.Model):
    timestamp = models.DateTimeField(auto_now_add=True)
    fisch_id = models.IntegerField()

    def __str__(self):
        return f"{self.timestamp} - {self.fisch_id}"
