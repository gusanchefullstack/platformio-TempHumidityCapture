#include <Arduino.h>
#include "arduino_secrets.h"
#include "WiFiNINA.h"
#include "Adafruit_Sensor.h"
#include "DHT.h"

#define DHTPIN 7
#define DHTTYPE DHT22

// put function declarations here:
void printWifiData();
void printSensorData(float, float, float);

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

DHT dht(DHTPIN, DHT22);

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  dht.begin();
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
    sensorPreviousTime = sensorCurrentTime;
    //To Do humidity Sampling
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if(isnan(h) || isnan(t)){
      Serial.println(F("Error reading sensor data for humidity and temperature"));
      return;
    }

    //Calculate heat index in Celsius
    float hic = dht.computeHeatIndex(t, h, false);
    printSensorData(h, t, hic);
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

//Print Sensor Data
void printSensorData(float h, float t, float hic) {
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(F("Heat index: "));
  Serial.print(hic);
  Serial.println(F("°C "));
}