#include <WiFi.h>
#include <WebSocketClient.h>
#include <string.h>
#define LED  23
struct wifiStruct {
// Home
const char* homeSsid = "Moc My";
const char* homePassword = "123456789";
char *homeHost = "192.168.1.112";
// Boarding House
const char* boardingSsid = "TP-LINK_D7E0";
const char* boardingPassword = "79377393";
char *boardingHost = "192.168.1.100";
};

struct wifiStruct wifiConfig;
const char* ssid     = wifiConfig.boardingSsid;
const char* password = wifiConfig.boardingPassword;


char path[] = "/";
char *host = wifiConfig.boardingHost;

const int PORT = 1406;
WebSocketClient webSocketClient;
WiFiClient client;

void connnect(){
   if (client.connect(host, PORT)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
  }
 
  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed.");
  }
}
void setup() {
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  delay(5000);
 
 connnect();
 
}
 
void loop() {
  String data;
  const String ON = "ON";
  const String OFF = "OFF"; 
  if (client.connected()) {
    Serial.println("Server connected");
    
    webSocketClient.sendData("Waiting get data...");
    webSocketClient.getData(data);
    if (data.length() > 0) {
      Serial.print("Received data: ");
      Serial.println(data);
      if (data == ON) {
        webSocketClient.sendData("#TurnedOn");
        digitalWrite(LED,HIGH);
      }
      if (data == OFF) {
        webSocketClient.sendData("#TurnedOff");
        digitalWrite(LED,LOW);
      }
    }
 
  } else {
    Serial.println("Client disconnected.");
    connnect();
  }
  delay(1000);
 
 
}
