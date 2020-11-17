#include<SoftwareSerial.h>

SoftwareSerial toArduino(D1,D2); // Rx and Tx pin
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  toArduino.begin(9600);
  delay(2000);
  Serial.println("Message is about to recieve from arduino");
}

void loop() {
  if(toArduino.available() > 0){
      Serial.println(toArduino.readStringUntil('\n'));
  }
}
