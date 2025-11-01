# myapp/urls.py
from django.urls import path
from .views import receive_esp_data, start_page, fisch

urlpatterns = [
    path('api/esp/', receive_esp_data, name='receive_esp_data'),
    path('', start_page, name='start_page'),
    path('fische/<int:fisch_id>/', fisch, name='fisch')
]
