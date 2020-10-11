#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int posArray[] = {10,45,90,135,170};


void setup() {
  myservo.attach(12);  // attaches the servo on pin 9 to the servo object
}

void loop() {
      for(int i = 0; i <= 3; i++){    // to go through the array from the begining to the end(excluding)
        myservo.write(posArray[i]);
        if(i==0)takeMe(posArray[i+1],posArray[i]);
        takeMe(posArray[i-1], posArray[i]);
        delay(1000);
      }

      for(int i = 4; i >= 1; i--){    // to go through the array from the last element to the begining(excluding)    
        takeMe(posArray[i-1], posArray[i]);
        delay(1000);
      }
 }
