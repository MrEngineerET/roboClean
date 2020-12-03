#include <Servo.h>
#include <SR04.h>

// servo variable
Servo myservo;  // create servo object to control a servo
int posArray[] = {10,45,90,135,170};

// ultrasonic variable
#define ECHO_PIN 11
#define TRIG_PIN 10
SR04 ultrasonic = SR04(ECHO_PIN, TRIG_PIN);
int distance[] = {0,0,0,0,0};


void setup() {
  Serial.begin(9600);
  myservo.attach(12);  // attaches the servo on pin 9 to the servo object
  delay(1000);
  Serial.println("ultrasonic and servo radar test program");
}

void loop() {
//       Test One
      for(int i = 0; i <= 3; i++){    // to go through the array from the begining to the end(excluding)
        myservo.write(posArray[i]);
        delay(100);
        distance[i] = ultrasonic.DistanceAvg();
        distance[i] > 50 ? distance[i] = 50 : false;  
        if( i == 0){
          Serial.print(distance[0]);
          Serial.print(", ");
          Serial.print(distance[1]);
          Serial.print(", ");
          Serial.print(distance[2]);
          Serial.print(", ");
          Serial.print(distance[3]);
          Serial.print(", ");
          Serial.println(distance[4]);
          }
      }

      for(int i = 4; i >= 1; i--){    // to go through the array from the last element to the begining(excluding)    
        myservo.write(posArray[i]);
        delay(100);
        distance[i] = ultrasonic.DistanceAvg();
        distance[i] > 50 ? distance[i] = 50 : false;  
        if(i == 4){
          Serial.print(distance[0]);
          Serial.print(", ");
          Serial.print(distance[1]);
          Serial.print(", ");
          Serial.print(distance[2]);
          Serial.print(", ");
          Serial.print(distance[3]);
          Serial.print(", ");
          Serial.println(distance[4]);
        }
      }

      //Test two
//      while(Serial.available()>0){
//          String posString = Serial.readString();    // readString function return a string sent from a serial communication
//          myservo.write(posString.toInt());   // writing the angular position to the servo
//          delay(200);    
//      }
//       Serial.println(ultrasonic.DistanceAvg());
//       delay(2000); 

      //Test three, for measuring a time required for measuring a distance
//       long sampleTime,delta;
//       for(int i = 0; i <= 3; i++){    // to go through the array from the begining to the end(excluding)
//        sampleTime = millis();
//        myservo.write(posArray[i]);
//        delay(10);
//        distance[i] = ultrasonic.DistanceAvg();
//        delta = millis()-sampleTime;
//        Serial.print("Time: ");
//        Serial.println(delta);
//      }
//
//      for(int i = 4; i >= 1; i--){    // to go through the array from the last element to the begining(excluding)    
//        sampleTime = millis();
//        myservo.write(posArray[i]);
//        delay(10);
//        distance[i] = ultrasonic.DistanceAvg();
//        delta = millis()-sampleTime;
//        Serial.print("Time: ");
//        Serial.println(delta);
//      }

}
