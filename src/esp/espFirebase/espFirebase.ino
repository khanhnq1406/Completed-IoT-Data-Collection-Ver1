#include "Arduino.h"
#include "time.h"
#include <string.h>
#include <stdlib.h>  
// ---------- FireBase --------------
#include <FirebaseESP8266.h>
#include  <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#define FIREBASE_HOST "https://iot-data-collection-5d7a4-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "AIzaSyCCMle_JkmnDA6A0aKU_7cZt5zZE905ggI"
#define WIFI_SSID "Yana Coffee Tang1" // Thay đổi tên wifi của bạn
#define WIFI_PASSWORD "184hoangdieu2" // Thay đổi password wifi của bạn
FirebaseData fbdo;
// --------------------------------------
unsigned int Status_Button,Status_1;
unsigned int Button_1,Button_2,Button_3,Button_4;
int data_B1,data_B2,data_B3,data_B4;
int Temp;
// -------------------------------------
// ---------- Date Time --------------
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;
String dateTime;

// -------------------------------------
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//The setup function is called once at startup of the sketch
void setup()
{
  Serial.begin(9600);
  delay(1000);
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Dang ket noi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println ("");
  Serial.println ("Da ket noi WiFi!");
  Serial.println(WiFi.localIP());

  // Init and get the time
  timeClient.begin();
  timeClient.setTimeOffset(25200);
  printLocalTime();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(100);;
}

// The loop function is called in an endless loop
void loop()
{
  srand (time(NULL));
  Temp = rand() % 100 + 1;
  printLocalTime();
  char ref[100] = "Temperature/";
  strcat(ref, dateTime.c_str());
  Firebase.setFloat( fbdo,ref, Temp);
  delay(1);
}
void printLocalTime(){
  timeClient.update();

  time_t epochTime = timeClient.getEpochTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
  
  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);  

  int currentHour = timeClient.getHours();
  Serial.print("Hour: ");
  Serial.println(currentHour);  

  int currentMinute = timeClient.getMinutes();
  Serial.print("Minutes: ");
  Serial.println(currentMinute); 
   
  int currentSecond = timeClient.getSeconds();
  Serial.print("Seconds: ");
  Serial.println(currentSecond);  


  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1;
  Serial.print("Month: ");
  Serial.println(currentMonth);

  int currentYear = ptm->tm_year+1900;
  Serial.print("Year: ");
  Serial.println(currentYear);

  //Print complete date:
  dateTime = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay) + " " 
  + String(currentHour)+ ":"  + String(currentMinute)+ ":" + String(currentSecond);
  Serial.print("Current date: ");
  Serial.println(dateTime);
  Serial.println("");

  delay(2000);
}
