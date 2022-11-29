/***************************************************
  Adafruit MQTT Library ESP8266 Example
  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino
  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


/************************* WiFi Access Point *********************************/

struct WifiConfig {
  // Home
  const char* homeSsid = "Moc My";
  const char* homePassword = "123456789";
  // Boarding House
  const char* boardingSsid = "TP-LINK_D7E0";
  const char* boardingPassword = "79377393";
  // Yana Coffee
  const char* yanaSsid = "Yana Coffee Tang1";
  const char* yanaPassword = "184hoangdieu2";
  // an
  const char* AnSsid = "an123";
  const char* AnPassword = "1237894567";
};
struct WifiConfig wifiConfig;
#define WLAN_SSID       wifiConfig.AnSsid
#define WLAN_PASS       wifiConfig.AnPassword

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "khanhnq1406"
#define AIO_KEY         "aio_KgOf14YMQ4fMr0AuvpFOWmu8OVGB"

/************ Global State (you don't need to change this!) ******************/

/************************* Firebase ******************************************/

#define FIREBASE_HOST "https://iot-data-collection-5d7a4-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "AIzaSyCCMle_JkmnDA6A0aKU_7cZt5zZE905ggI"
/************************* Firebase ******************************************/
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiClientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/mqtt-server");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/mqtt-server");
