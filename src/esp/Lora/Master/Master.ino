/*
 * LoRa E32-TTL-100
 * Receive fixed transmission message as a specified point.
 * https://www.mischianti.org
 *
 * E32-TTL-100----- Arduino UNO or esp8266
 * M0         ----- 3.3v (To config) GND (To send) 7 (To dinamically manage)
 * M1         ----- 3.3v (To config) GND (To send) 6 (To dinamically manage)
 * TX         ----- PIN 2 (PullUP)
 * RX         ----- PIN 3 (PullUP & Voltage divider)
 * AUX        ----- Not connected (5 if you connect)
 * VCC        ----- 3.3v/5v
 * GND        ----- GND
 *
 */
#include "Arduino.h"
#include "LoRa_E32.h"
#include "time.h"
#include <string.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <FirebaseESP8266.h>-
#include <SoftwareSerial.h>
#include "config.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
FirebaseData fbdo;
/************ Esp8266 PIN ******************/

SoftwareSerial mySerial(D2, D3); // e32 TX e32 RX
LoRa_E32 e32ttl(&mySerial, D5, D7, D6);

/************ Esp8266 PIN ******************/

/************ Data ******************/
unsigned int Status_Led;
//bool Led_1,Led_2,Led_3,Led_4;
int data_L1,data_L2,data_L3,data_L4;
int StatusLed_1,StatusLed_2,StatusLed_3,StatusLed_4;
int Temp;
int Code_Data=0;
int Code_Data_S=0;
int Status_Sv=0;
int Status_Send=0;
int ID_Slave_Send=0;
unsigned long Time_=0,Time2_=0;
int flag1=0,flag2=0;
struct Message {
//    char type[5];
//    char message[8];
    byte Temperature_R[4];
    byte Status_Led_R[4];
    byte Status_Led_SV[4];
    byte Mode_R[4];
};
// ---------- Date Time --------------
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;
String dateTime;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

/****************************************/
uint32_t x=0;
bool hasTurnedOnLight1, hasTurnedOnLight2;
/************ MQTT******************/
// --------------------------------
void printParameters(struct Configuration configuration);
void printModuleInformation(struct ModuleInformation moduleInformation);
void MQTT_connect();

void setup()
{
  Serial.begin(9600);
  delay(1000);
  
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

  Serial.println ("");
  Serial.println ("Da ket noi WiFi!");
  Serial.println(WiFi.localIP());

  // Init and get the time
  timeClient.begin();
  timeClient.setTimeOffset(25200);
  printLocalTime();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(100);
  mqtt.subscribe(&sub);

/************ LORa ******************/
    e32ttl.begin();
//  e32ttl.resetModule();
    // After set configuration comment set M0 and M1 to low
    // and reboot if you directly set HIGH M0 and M1 to program
    ResponseStructContainer c;
    c = e32ttl.getConfiguration();
    Configuration configuration = *(Configuration*) c.data;
    configuration.ADDL = 3;
    configuration.ADDH = 0;
    configuration.CHAN = 8;
    configuration.SPED.uartBaudRate = UART_BPS_9600;
    configuration.SPED.uartParity = MODE_00_8N1; // Parity bit
    configuration.SPED.airDataRate = AIR_DATA_RATE_100_96 ;// Air data rate
    configuration.OPTION.transmissionPower = POWER_20;
    configuration.OPTION.fec = FEC_0_OFF;
    configuration.OPTION.fixedTransmission = FT_FIXED_TRANSMISSION;
    configuration.OPTION.wirelessWakeupTime = WAKE_UP_750;
    configuration.OPTION.ioDriveMode = IO_D_MODE_PUSH_PULLS_PULL_UPS;
    e32ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
    printParameters(configuration);
    ResponseStatus resetModule();
    // ---------------------------
    Serial.println();
    Serial.println("Start listening!");

} 
void loop()
{
  if(Status_Sv == 1)
  {
    Code_Data_S=103;
  }
  else
  {
    Code_Data_S=100;
  }
  if(Status_Send == 0)
  {
    if(flag1 == 0) // first state
    {
      flag1=1;
      Time_ = millis();
    }
    if ( (unsigned long) (millis() - Time_) > 500)// gui du lieu x ms
    {
      Status_Send =1;
      flag1=0;
    }
    else
    {
      SendData_Slave(0,1,0x04);
      Serial.println("            send data ");
    }
  }
  else
  {
    if(flag2  == 0) // first state
    {
      flag2=1;
      Status_Sv=0;
      Time2_=millis();
    }
    if ( (unsigned long) (millis() - Time2_) > 400)// doc du lieu x ms
    {
      Status_Send =0;
      flag2=0;
    }
    else 
    {
      ReadData_Slave();
      Serial.println("              read data ");
    }
  }
  if(Code_Data == 101)
  {
    Code_Data=0;
    Status_Sv=0;
    Status_Send=0;
    Serial.println("reset ");
  }
  printLocalTime();
  FireBase();
  MQTT();
  Serial.print("Status_Sv  = "); Serial.println(Status_Sv );
  Serial.print("Status_Send  = "); Serial.println(Status_Send );
}
void SendData_Slave(int ADDL, int ADDH ,int CHAN )
{
    struct Message {
//      char type[5] = "TEMP";
//      char message[8] = "Kitchen";

    byte Temperature_R[4];
    byte Status_Led_R[4];
    byte Status_Led_SV[4];
    byte Mode_R[4];// khi gui du lieu can gui ma 101 cho master|| khi nhan 100 tuc phai gui du lieu cho master
  } message;
  
//  *(int*)(message.Temperature_R) = Temp;
  *(int*)(message.Status_Led_SV)= Merge_DataLed();
  *(int*)(message.Mode_R)= Code_Data_S;
    Serial.print("Merge_DataLed = ");
    Serial.println(Merge_DataLed());
  ResponseStatus rs = e32ttl.sendFixedMessage(ADDL, ADDH, CHAN,&message, sizeof(Message));
}
void ReadData_Slave()
{
    if (e32ttl.available()  > 1){
  
    ResponseStructContainer rsc = e32ttl.receiveMessage(sizeof(Message));
    struct Message message = *(Message*) rsc.data;
    
    Status_Led= *(int*)(message.Status_Led_R);
    Temp=*(int*)(message.Temperature_R);
    Code_Data=*(int*)(message.Mode_R);
    rsc.close();
    Decode(); 
    Serial.print("Led_1 = ");
    Serial.println(StatusLed_1);
    Serial.print("Led_2 = ");
    Serial.println(StatusLed_2);
    Serial.print("Led_3 = ");
    Serial.println(StatusLed_3);
    Serial.print("Led_4 = ");
    Serial.println(StatusLed_4);
    Serial.print("TEMP = ");
    Serial.println(Temp);
    Serial.print("CodeData = ");
    Serial.println(Code_Data);
  }
}
void MQTT()
{
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(100))) {
    if (subscription == &sub) {
//      Serial.print(F("Got: "));
//      Serial.println((char *)sub.lastread);
      if (strcmp((char *)sub.lastread,"#turnOn") == 0) {
//        Serial.println("Turned ON 1");
        pub.publish("#turnedOn");
        StatusLed_1 = 1;
        Status_Sv=1;
      }
      if (strcmp((char *)sub.lastread,"#turnOff") == 0) {
//        Serial.println("Turned OFF 1");
        pub.publish("#turnedOff");
        StatusLed_1 = 0;
        Status_Sv=1;
      }
      if (strcmp((char *)sub.lastread,"#turnOnLight2") == 0) {
        Serial.println("Turned ON Light 2");
        pub.publish("#turnedOnLight2");
        StatusLed_2 = 1;
        Status_Sv=1;
      }
      if (strcmp((char *)sub.lastread,"#turnOffLight2") == 0) {
        Serial.println("Turned OFF Light 2");
        pub.publish("#turnedOffLight2");
        StatusLed_2 = 0;
        Status_Sv=1;
      }
      if (strcmp((char *)sub.lastread,"#checkStatusLight1") == 0) {
//        Serial.println("Check status light 1");
        if (StatusLed_1)
          pub.publish("#hasturnedOn");
        else
          pub.publish("#hasturnedOff");
      }
      if (strcmp((char *)sub.lastread,"#checkStatusLight2") == 0) {
//        Serial.println("Check status light 2");
        if (StatusLed_2)
          pub.publish("#hasturnedOn");
        else
          pub.publish("#hasturnedOff");
      }
      if (strcmp((char *)sub.lastread,"#checkStatus") == 0) {
//        Serial.println("Check status");
        String statusLight1, statusLight2;
        if (StatusLed_1) {
          statusLight1 = "true";
        }
        else {
          statusLight1 = "false";
        }
        if (StatusLed_2) {
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
}
void FireBase()
{
  srand (time(NULL));
  char ref[100] = "Temperature/";
  Firebase.setFloat ( fbdo,"Led 4", StatusLed_4);
  Firebase.setFloat ( fbdo,"Led 3", StatusLed_3);
  Firebase.setFloat ( fbdo,"Led 2", StatusLed_2);
  Firebase.setFloat ( fbdo,"Led 1", StatusLed_1);
  strcat(ref, dateTime.c_str());
  Firebase.setFloat( fbdo,ref, random(0,230));

  delay(1);
}
void Decode()
{
  if( (Status_Led & 0b01)  == 0b01)
  {
    StatusLed_1 = HIGH;
  }
  else {
    StatusLed_1 = LOW;
  }
  if( (Status_Led & 0b10)  == 0b10)
  {
    StatusLed_2 = HIGH;
  }
  else {
    StatusLed_2 = LOW;
  }
  if((Status_Led & 0b100)  == 0b100  )
  {
    StatusLed_3 = HIGH;
  }
  else {
    StatusLed_3 = LOW;
  }
  if((Status_Led & 0b1000 ) == 0b1000)
  {
    StatusLed_4 = HIGH;
  }
  else {
    StatusLed_4 = LOW;
  }
}
void Data_Led()
{
  if(StatusLed_1)
  {
    data_L1=0x01;
  }
  else
  {
    data_L1=0x00;
  }
  if(StatusLed_2)
  {
    data_L2=0b10;
  }
  else
  {
    data_L2=0x00;
  }
  if(StatusLed_3)
  {
    data_L3=0b100;
  }
  else
  {
    data_L3=0x00;
  }
  if(StatusLed_4)
  {
    data_L4=0b1000;
  }
  else
  {
    data_L4=0x00;
  }
}
int Merge_DataLed()
{
   Data_Led();
   unsigned int Status_LedS;
   Status_LedS = data_L1 | data_L2 | data_L3 | data_L4;
//   Serial.println(Status_Led);
   return Status_LedS;
}
void printParameters(struct Configuration configuration) {
    Serial.println("----------------------------------------");
 
    Serial.print(F("HEAD : "));  Serial.print(configuration.HEAD, BIN);Serial.print(" ");Serial.print(configuration.HEAD, DEC);Serial.print(" ");Serial.println(configuration.HEAD, HEX);
    Serial.println(F(" "));
    Serial.print(F("AddH : "));  Serial.println(configuration.ADDH, DEC);
    Serial.print(F("AddL : "));  Serial.println(configuration.ADDL, DEC);
    Serial.print(F("Chan : "));  Serial.print(configuration.CHAN, DEC); Serial.print(" -> "); Serial.println(configuration.getChannelDescription());
    Serial.println(F(" "));
    Serial.print(F("SpeedParityBit     : "));  Serial.print(configuration.SPED.uartParity, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTParityDescription());
    Serial.print(F("SpeedUARTDatte  : "));  Serial.print(configuration.SPED.uartBaudRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTBaudRate());
    Serial.print(F("SpeedAirDataRate   : "));  Serial.print(configuration.SPED.airDataRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getAirDataRate());
 
    Serial.print(F("OptionTrans        : "));  Serial.print(configuration.OPTION.fixedTransmission, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getFixedTransmissionDescription());
    Serial.print(F("OptionPullup       : "));  Serial.print(configuration.OPTION.ioDriveMode, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getIODroveModeDescription());
    Serial.print(F("OptionWakeup       : "));  Serial.print(configuration.OPTION.wirelessWakeupTime, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getWirelessWakeUPTimeDescription());
    Serial.print(F("OptionFEC          : "));  Serial.print(configuration.OPTION.fec, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getFECDescription());
    Serial.print(F("OptionPower        : "));  Serial.print(configuration.OPTION.transmissionPower, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getTransmissionPowerDescription());
    Serial.print(F("MODE        : "));Serial.println(e32ttl.getMode());
    
    Serial.println("----------------------------------------");
 
}
void printModuleInformation(struct ModuleInformation moduleInformation) {
    Serial.println("----------------------------------------");
    Serial.print(F("HEAD BIN: "));  Serial.print(moduleInformation.HEAD, BIN);Serial.print(" ");Serial.print(moduleInformation.HEAD, DEC);Serial.print(" ");Serial.println(moduleInformation.HEAD, HEX);
 
    Serial.print(F("Freq.: "));  Serial.println(moduleInformation.frequency, HEX);
    Serial.print(F("Version  : "));  Serial.println(moduleInformation.version, HEX);
    Serial.print(F("Features : "));  Serial.println(moduleInformation.features, HEX);
    Serial.println("----------------------------------------");
 
}

void printLocalTime(){
  timeClient.update();

  time_t epochTime = timeClient.getEpochTime();
//  Serial.print("Epoch Time: ");
//  Serial.println(epochTime);
  
  String formattedTime = timeClient.getFormattedTime();
//  Serial.print("Formatted Time: ");
//  Serial.println(formattedTime);  

  int currentHour = timeClient.getHours();
//  Serial.print("Hour: ");
//  Serial.print(currentHour);  
  int currentMinute = timeClient.getMinutes();
//  Serial.print("Minutes: ");
//  Serial.println(currentMinute); 
  String Time_;
  int currentSecond = timeClient.getSeconds();
//  Serial.print("Seconds: ");
//  Serial.println(currentSecond);  
  Time_ =  String(currentHour) + ":" + String(currentMinute) + ":" + String(currentSecond);
  Serial.println(Time_); 
  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
//  Serial.print("Month day: ");
//  Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1;
//  Serial.print("Month: ");
//  Serial.println(currentMonth);

  int currentYear = ptm->tm_year+1900;
//  Serial.print("Year: ");
//  Serial.println(currentYear);

  //Print complete date:
  dateTime = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay) + " " 
  + String(currentHour)+ ":"  + String(currentMinute)+ ":" + String(currentSecond);
//  Serial.print("Current date: ");
//  Serial.println(dateTime);
//  Serial.println("");

//  delay(2000);
}
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
       delay(1);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
