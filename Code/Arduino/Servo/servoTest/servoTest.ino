/* This is a test code for the servo motor I will use (SG90)
 *  
 *  Test one
 *    a specified value of angular position will be provided using serial port
 *  Test two
 *    a continues 180 degree sweep 
 *  Test three
 *    continuesly directing the servo to five specified angular direction 
 */   

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

String posString;   // a string to hold the position sent from serial communication
int posArray[] = {10,45,90,135,170};


void setup() {
  Serial.begin(9600);
  myservo.attach(12);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  // Test one
//  while(Serial.available()>0){
//    posString = Serial.readString();    // readString function return a string sent from a serial communication
//    myservo.write(posString.toInt());   // writing the angular position to the servo
//    delay(200);    
//  }

  // Test two
  for(int pos = 0; pos <= 180; pos++){
    myservo.write(pos);
    delay(10);    // determines the speed of the CCW sweep
  }

  for (int pos = 180; pos >=0; pos--){
    myservo.write(pos);
    delay(10);    // determines the speed of the CW sweep
  }

  // Test three
//      for(int i = 0; i <= 3; i++){    // to go through the array from the begining to the end(excluding)
//        myservo.write(posArray[i];
//        delay(1000);
//      }
//
//      for(int i = 4; i >= 1; i--){    // to go through the array from the last element to the begining(excluding)    
//        myservo.write(posArray[i];
//        delay(1000);
//      }
 }
