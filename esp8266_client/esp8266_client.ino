#define BLYNK_TEMPLATE_ID "TMPL2aWK7wvS4"
#define BLYNK_TEMPLATE_NAME "Gas Detector"
#define BLYNK_AUTH_TOKEN "0yzooMDGsrwou7BD8sX_9Z2W5_QWBw84"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <UrlEncode.h>
#include <Callmebot_ESP8266.h>
#include "whatsapp.h"

// WhatsApp API
String phoneNumber = "+2348107739669";
String apiKey = "2245128";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "gas_detector";
char pass[] = "detector_2024";
unsigned long gasThreshold = 500; // in ppm
unsigned long tempThreshold = 50; 
unsigned short temp, gas = 0; 

WidgetLED led1(V0);

BlynkTimer timer;

// V3 LED Widget is blinking
static byte count = 0;
void blinkLedWidget()
{ 
  if(Serial.available()> 0){
    String payload = Serial.readString();
    payload.trim();
    byte indexOfDelimiter = payload.indexOf(",");
    String gasPayload = payload.substring(0, indexOfDelimiter);
    String temperaturePayload = payload.substring(indexOfDelimiter + 1);
  
    temp = temperaturePayload.toInt();
    gas = gasPayload.toInt();
    if(gas >= gasThreshold){
        Serial.println("Gas danger");
        led1.on();
        String message = "Gas leakage detected!" + String(" Gas level is ") + String(gas) + String(" ppm");
        Blynk.logEvent("gas_leakage", message);
        Callmebot.whatsappMessage(phoneNumber, apiKey, message);
        delay(1000);
    }else{
        led1.off();  
    }
    if(temp >= tempThreshold){
        Serial.println("temp danger");
        String message = String("High temperature detected!") +  String(" Temperature is ") + String(temp) + String(" degree celcius"); 
        Blynk.logEvent("high_temp", message); 
        Callmebot.whatsappMessage(phoneNumber, apiKey, message);
        delay(1000);
      } 
    Blynk.virtualWrite(V1, temp);
    Blynk.virtualWrite(V2, gas);
   }
}

void setup()
{
  // Debug console
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Callmebot.begin();
  timer.setInterval(1500L, blinkLedWidget);
  Serial.begin(9600);
}


void loop()
{
  Blynk.run();
  timer.run();
}
