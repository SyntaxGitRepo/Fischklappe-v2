# Fischklappe v2
Das ist das Repository von einem Projekt für den Tag der offenen Türe der HTL Reutte.

## Idee
Unser Professor hatte die Idee eine Fischklappe zu bauen. 
Die Überlegung war ein Glasrohr mit einem Durchmesser von 11 cm zu nehemen und mittles 3D-Druck eine Klappe an ein Ende zu montieren. Diese sollte mit einem Servo geöffnet werden können.
In unserer Klasse sind einige Plüsch Fische verteilt gewesen. Der Gedanke war, diese Fische mit kleinen RFID-Tags zu Chipen. Dann kann an einem Ende des Glasrohres eine RFID-Empfängerspule
mit Auswertungschip angeschlossen werden und die Daten vom Fisch ausgelesen werden. Ein Mikroprozessor soll dann diese Daten verarbeiten und feststellen ob der Fisch durch die Klappe darf.
Wenn dies der Fall ist soll der Servo angesteuert werden und die Klappe auf gehen. Außerdem sollen einige LEDs von einem LED-strip in der Farbe des Fisches aufleuchten. 
Unter den Fischen war auch ein Krokodil. Bei dem sollte der LED-Strip blinken und die Klappe nicht aufgehen.

## Material 
 * Glasrohr (1.5m x Ø 11cm)
 * 3D-Drucker für die Klappe
 * UNO R3 + ATmeg328p + ESP8266 WIFI (4Mb) USB - TTL
 * Wl-134 RFID-Chip
 * Sg90 180° Servo
 * (Adafruit NeoPixel WS2812 LED-Strip)
 * Adafruit NeoPixel WS2812 LED-Ring
 * Jumpers
 * 2 kleine Steckbretter für leichtere Verkabelung
 * 9V-Batterie
 * Großer Kabelkanal als Halterung

## Software
* Arduino IDE (ESP8266)
* VSCode (ESP8266)
* Siemens NX (3D-Zeichnungen)

## Aufbau
Das Schaltbild für die Fischklappe:
![Fischklappe Schaltplan](https://github.com/user-attachments/assets/f88d5c95-cfa9-4f02-bcf7-0b865ea993cb)








/**
 *To connect to Arduino via Serial (USB) go to File->Preferences and paste "http://arduino.esp8266.com/stable/package_esp8266com_index.json"
 *into the additional boards manager URL and press OK. Then you should be able to go to the board manager (on the left of the screen) and search "esp8266".
 *install the board and select it by going to Tools->Board:->esp8266->Generic ESP8266 Module.
 *To upload code via OTA (Over The Air) to the ESP go to Tools->Port->Network Ports and select the ESP.
 *Before you upload code you need to reset the ESP (red hardware button) and then click the upload button. The long red Text and the beginning is not an error.
 *It will ask you for a password which is "admin". After uploading there will be an error message that you can ignore.
 *If you want to connect to the ESP via Wi-Fi to see the output from the WL-134 (RFID-Chip) you can do that with PuTTY. enter the ip and select connection type other: Telnet.
 *To connect to the RFID-Chip: connect 5V to the Power and GND to the ESP-Pins. Connect the TxD-Pin from the RFID-Chip to GPIO-12 of the ESP.
**/
