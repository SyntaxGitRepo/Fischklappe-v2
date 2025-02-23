// Fischklappe v2
// Version: v2
// Author: Elias Wassermann

#include <Arduino.h>
#include <ESP8266WiFi.h>  
#include <ArduinoOTA.h>
#include <SoftwareSerial.h>
#include <Rfid134.h>
#include <TelnetStream.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

// The SSID and Password for the Wi-Fi connection (change these in the File "wifiCredentials.h")
#include "wifiCredentials.h"

const char* ssid =      SECRET_SSID;
const char* password =  SECRET_PASS;

// The Pins of all external Hardware
#define SERVO_PIN 2
#define RFID_PIN  4
#define LED_PIN   12

// number of leds on the led-strip  
#define LED_COUNT 12

// a simple struct for a rgb color
struct color
{
  int red;
  int green;
  int blue;
};

/**
  * A struct for all data of a fish
  * IDOfFish: The ID of the fish (rfid-id). integer
  * fishColor: The color of a fish (rgb). color
  * isAllowedThrough: true if the fish is allowed through the door; otherwise false
 */
struct fishData
{
  int IDOfFish;
  color fishColor;
  bool isAllowedThrough;
};

/**
 * A simple struct that stores all fish and implements functions.
 * fishData: An array of all fish.
 * currentIndex: the current index of where the next fish will be added.
 * bool contains(): returns if the fish is already in the array (based in IDOfFish); returns false otherwise
 * fishData getFishByID(): returns a fishData struct of the fish with the ID specified. returns an empty struct if not found
 * void addFish(): adds a new fish at currentIndex with data given.
 */
struct fish {
  fishData fishArray[50];
  int currentIndex = 0;

  bool contains(const int IDOfFish) {
    for (int i = 0; i < 50; i++) {
      if (IDOfFish == fishArray[i].IDOfFish) {
        return true;
      }
    }
    return false;
  }

  fishData getFishByID(const int IDOfFish) {
    for (int i = 0; i < 50; i++) {
      if (fishArray[i].IDOfFish == IDOfFish) {
        return fishArray[i];
      }
    }
    return {};
  }

  void addFish(fishData fish) {
    fishArray[currentIndex] = fish;
    currentIndex++;
  }
};

// The fish struct with all fish stored
fish fish;

// define the servo and led-strip
Servo servo;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

/*
The setStripColor method sets all the leds on  the led-strip to the color specified.
There is a 1ms delay between setting the color of each led. 
*/ 
void setStripColor(color color) 
{
  for(int i = 0; i < LED_COUNT; i++)
  {
    strip.setPixelColor(i, strip.Color(color.red, color.green, color.blue));
    strip.show();
    delay(1);
  }
}

/**
The blinkStrip method blinks the led-strip with a color specified.
Each Blink will take a given time (timePerBlink) and will happen as often as specified (numOfBlinks)
**/
void blinkStrip(color color, int timePerBlink, int numOfBlinks)
{
  for (int i = 0; i < numOfBlinks; i++)
  {
    setStripColor(color);
    delay(timePerBlink/2);

    setStripColor({0,0,0});
    delay(timePerBlink/2);
  }
}

class RfidNotify
{
public:
  // If an error occured during reading of a frame from the rfid-chip
  static void OnError(Rfid134_Error errorCode)
  {
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }

  // Is called if a packet from the rfid-chip has been read
  static void OnPacketRead(const Rfid134Reading& reading)
  {
    // the ID of the Fish (rfid-tag). (The last 4 binary digits)
    int IDOfFish = reading.id & 63;

    // Print out the id to the serial port and all telnet connections
    TelnetStream.println(IDOfFish);
    Serial.println(IDOfFish);

    if (!fish.contains(IDOfFish)) return;

    fishData currentFish = fish.getFishByID(IDOfFish);

    if (currentFish.isAllowedThrough)
    {
      setStripColor(currentFish.fishColor);  // set all leds of the ledstrip to the color specified

      // open the door with the servo and close it again after 1.5s
      servo.write(0);
      delay(4000);

      for (int i = 0; i <= 180; i++)
      {
        servo.write(i);
        delay(1);
      }
      setStripColor({0,0,0});
    } else
    {
      blinkStrip(currentFish.fishColor, 1000, 3);
    }
  }
};

SoftwareSerial secondarySerial(RFID_PIN, 16); // RX, TX
Rfid134<SoftwareSerial, RfidNotify> rfid(secondarySerial);

void setup() {
  // setup serial
  Serial.begin(9600);
  Serial.println("Booting");

  // setup servo
  servo.attach(SERVO_PIN);  
  servo.write(180);

  // setup wifi connection to local network (credentials given in the file "wifiCredentials.h")
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) // Restart ESP if no connection to the network has been achieved (ex. wrong credentials)
  { 
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Set Hostname to "ESP8266"
  ArduinoOTA.setHostname("ESP8266");

  // Set Password for connecting via OTA to "admin"
  ArduinoOTA.setPassword("admin");

  // if code is uploaded via OTA print out progress bar (percent) to Serial.
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) 
  {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  // Handle Error occured while Uploading/Connecting OTA
  ArduinoOTA.onError([](ota_error_t error) 
  {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });

  // Start OTA, Telnet, serial to WL-134, led strip and rfid 
  ArduinoOTA.begin();
  TelnetStream.begin();
  secondarySerial.begin(9600);
  rfid.begin();

  // start led-strip and configure it
  strip.begin();
  strip.show();
  strip.setBrightness(50);

  // Add all fish known to the fish struct array
  fish.addFish({63, {180, 32, 65}, true});
  fish.addFish({1, {136, 0, 255}, true});
  fish.addFish({5, {255, 0, 0}, false});
  fish.addFish({8, {255, 0, 0}, true});

  // print IP to Serial
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  ArduinoOTA.handle();
  rfid.loop();
}
