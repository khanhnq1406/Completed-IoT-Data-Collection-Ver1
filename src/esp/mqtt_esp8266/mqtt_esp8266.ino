#include "config.h"
#include <string.h>
/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&sub);
}

uint32_t x=0;
bool hasTurnedOnLight1, hasTurnedOnLight2;
void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(100))) {
    if (subscription == &sub) {
      Serial.print(F("Got: "));
      Serial.println((char *)sub.lastread);
      if (strcmp((char *)sub.lastread,"#turnOn") == 0) {
        Serial.println("Turned ON");
        pub.publish("#turnedOn");
        hasTurnedOnLight1 = true;
      }
      if (strcmp((char *)sub.lastread,"#turnOff") == 0) {
        Serial.println("Turned OFF");
        pub.publish("#turnedOff");
        hasTurnedOnLight1 = false;
      }
      if (strcmp((char *)sub.lastread,"#turnOnLight2") == 0) {
        Serial.println("Turned ON Light 2");
        pub.publish("#turnedOnLight2");
        hasTurnedOnLight2 = true;
      }
      if (strcmp((char *)sub.lastread,"#turnOffLight2") == 0) {
        Serial.println("Turned OFF Light 2");
        pub.publish("#turnedOffLight2");
        hasTurnedOnLight2 = false;
      }
      if (strcmp((char *)sub.lastread,"#checkStatusLight1") == 0) {
        Serial.println("Check status light 1");
        if (hasTurnedOnLight1)
          pub.publish("#hasturnedOn");
        else
          pub.publish("#hasturnedOff");
      }
      if (strcmp((char *)sub.lastread,"#checkStatusLight2") == 0) {
        Serial.println("Check status light 2");
        if (hasTurnedOnLight2)
          pub.publish("#hasturnedOn");
        else
          pub.publish("#hasturnedOff");
      }
      if (strcmp((char *)sub.lastread,"#checkStatus") == 0) {
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
        char* status_char;
        // strcpy(status_char, status);
        pub.publish(status.c_str());
      }
    }
  }
  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
