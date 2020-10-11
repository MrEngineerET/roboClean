/* This is a test code for the ultrasonic sensor I will use (HC-SR04)
 *  
 *  Test one
 *    Printing out the measured distance continuesly (1 second interval) 
 */   

#include <SR04.h>

#define ECHO_PIN 11
#define TRIG_PIN 10

SR04 ultrasonic = SR04(ECHO_PIN, TRIG_PIN);

int distance;

void setup() {
  Serial.begin(9600);
}

void loop() {
  distance = ultrasonic.DistanceAvg();
  if(distance < 4 || distance > 400){ // if the distance calculated is not valid( HC-SR04 can only measure distance between 4cm to 400cm)
    distance = 0;  // this is an indication of the distance which is invalid
  }
  Serial.print("Distance measured is: ");
  Serial.print(distance);
  Serial.println("cm");
  delay(1000);
}
