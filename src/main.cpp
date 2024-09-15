#include <Arduino.h>
#include "arduino_secrets.h"
#include "WiFiNINA.h"

// put function declarations here:
void printWifiData();

//put global variables here:
char ssid[] = SECRET_SSID;
char pswd[] = SECRET_PASS;

const int LED = 5;
int ledState = LOW;

unsigned long ledOnoffTime = 2000;
unsigned long sensorSamplingTime = 60000;
unsigned long wifiReconnectTime = 30000;
int wifiStatus = WL_IDLE_STATUS;

unsigned long ledPreviousTime = 0;
unsigned long sensorPreviousTime = 0;

bool print_info = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  //Trying to connect to wifi before sending humidity and temp data
  while(wifiStatus != WL_CONNECTED){
    Serial.print("Attempting to connect to WIFI: ");
    Serial.println(ssid);
    wifiStatus = WiFi.begin(ssid, pswd);
    if(wifiStatus == WL_CONNECTED){
      print_info = true;
    }
    delay(wifiReconnectTime);
  }
  if(wifiStatus == WL_CONNECTED && print_info){
    Serial.println("Connected to network successfully!!!");
    printWifiData();
    print_info = false;
  }

  unsigned long sensorCurrentTime = millis();
  unsigned long ledCurrentTime = millis();

  if((sensorCurrentTime - sensorPreviousTime) > sensorSamplingTime){
    //To Do humidity Sampling
  }

  if((ledCurrentTime - ledPreviousTime) >= ledOnoffTime){
    ledPreviousTime = ledCurrentTime;
    
    if(ledState == LOW){
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(LED, ledState);
  }

}

// put function definitions here:

//Print wifi information on serial port
void printWifiData() {
  Serial.println("*************************************************");
  Serial.println("***************Network IP information************");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("IP Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("IP Gateway: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("Signal strenght RSSI: ");
  Serial.println(WiFi.RSSI());
  Serial.println("*************************************************");
}