#include <Wire.h> // this is part of arduino
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include "temperature.h"
#include "gas.h"


Servo servo;  // create servo object to control a servo

byte const rx  = 3; // pin connected to esp8266 RX pin 
byte const tx  = 8; // pin connected to esp8266 TX pin 
byte const thermistorPin = A0;
byte const servoPin = 7;
byte const greenLED = 11; 
byte const redLED = 12; 
byte const buzzerPin = 10; 

byte const gasTreshold = 500;
byte const tempTreshold = 50; 
bool isDangerFlag = false;
byte temp = 0;
unsigned short gasVal = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);// display 1

static byte count = 0;
void openValve (){
  servo.write(180);
}

void closeValve (){
  servo.write(0);
}

void setup() {
  servo.attach(servoPin);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  closeValve();
  lcd.begin();//initialize LCD2
  lcd.backlight();// turn the backlight ON for the LCD1
  lcd.setCursor(0,0);
  lcd.print("Gas leakage");
  lcd.setCursor(0,1);
  lcd.print("detector");
  initMQ2(); // initialize MQ2 module
  delay(1000);
  lcd.clear();
}

void loop() { 
      temp = getTemperatureValue(thermistorPin);
      gasVal = readLPGppm(); // returns lpg in ppm
      lcd.setCursor(0, 0);
      lcd.print("Temperature: " + String(temp)+ String("C"));
      lcd.setCursor(0, 1);
      lcd.print("LPG: " + String(gasVal) + String("ppm"));
      Serial.println(String(gasVal) + "," + String(temp));
      delay(1000);
      lcd.clear();
      
       if(gasVal >= gasTreshold || temp >= tempTreshold){
          closeValve();
          // turn on buzzer
          digitalWrite(greenLED, LOW);
          if(isDangerFlag == true){
            digitalWrite(redLED, LOW);
            digitalWrite(buzzerPin, LOW);
            isDangerFlag = false;
            }else{
            digitalWrite(redLED, HIGH);  
            digitalWrite(buzzerPin, HIGH);
            isDangerFlag = true;
           }
          lcd.setCursor(0, 0);
          lcd.print("Danger Detected!");
          lcd.setCursor(4, 1);
          lcd.print("Gas/Fire");
          delay(800);
          lcd.clear();
        }else{
          digitalWrite(greenLED, HIGH);
          digitalWrite(redLED, LOW);
          digitalWrite(buzzerPin, LOW);
          openValve();
        }
}
