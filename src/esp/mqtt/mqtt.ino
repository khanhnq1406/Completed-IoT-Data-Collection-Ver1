#include <WiFi.h>
#include <string.h>

#define LED  23
/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

// this int will hold the current count for our sketch
bool hasTurnedOnLight1, hasTurnedOnLight2;
#define IO_LOOP_DELAY 5000
unsigned long lastUpdate = 0;

// set up the feed
AdafruitIO_Feed *mqttServer = io.feed("mqtt-server");

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  mqttServer->onMessage(handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  mqttServer->get();

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
}

// this function is called whenever a 'mqttServer' message
// is received from Adafruit IO. it was attached to
// the mqttServer feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");
  Serial.println(data->value());
  if (strcmp(data->value(),"#turnOn") == 0) {
    Serial.println("Turned ON");
    mqttServer->save("#turnedOn");
    hasTurnedOnLight1 = true;
  }
  if (strcmp(data->value(),"#turnOff") == 0) {
    Serial.println("Turned OFF");
    mqttServer->save("#turnedOff");
    hasTurnedOnLight1 = false;
  }
   if (strcmp(data->value(),"#turnOnLight2") == 0) {
    Serial.println("Turned ON Light 2");
    mqttServer->save("#turnedOnLight2");
    hasTurnedOnLight2 = true;
  }
  if (strcmp(data->value(),"#turnOffLight2") == 0) {
    Serial.println("Turned OFF Light2");
    mqttServer->save("#turnedOffLight2");
    hasTurnedOnLight2 = false;
  }
  if (strcmp(data->value(),"#checkStatusLight1") == 0) {
    Serial.println("Check status light 1");
    if (hasTurnedOnLight1)
      mqttServer->save("#hasturnedOn");
    else
      mqttServer->save("#hasturnedOff");
  }
  if (strcmp(data->value(),"#checkStatusLight2") == 0) {
    Serial.println("Check status light 2");
    if (hasTurnedOnLight2)
      mqttServer->save("#hasturnedOn");
    else
      mqttServer->save("#hasturnedOff");
  }
  if (strcmp(data->value(),"#checkStatus") == 0) {
    Serial.println("Check status");
    String statusLight1, statusLight2;
    if (hasTurnedOnLight1) {
      statusLight1 = "true";
    }
    else {
      statusLight1 = "false";
    }
    if (hasTurnedOnLight2) {
      statusLight2 = "true";
    }
    else {
      statusLight2 = "false";
    }
    String status = statusLight1 + "~" + statusLight2;
    mqttServer->save(status);
  }
}
