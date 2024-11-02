#define LED  2
void blinky(byte i){
  for(byte count = 0; count < i; count++){
       digitalWrite(LED, HIGH);
       delay(1000);
       digitalWrite(LED, LOW);
       delay(1000); 
    }
 }
void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 pinMode(LED, OUTPUT);
 blinky(1);
}

void loop() {
  // put your main code here, to run repeatedly:
if(Serial.available()> 0){
  blinky(2);
  if(Serial.readString().startsWith("t:")){
   blinky(5);
   }
  }
}
