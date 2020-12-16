#include <Servo.h>
#include <SR04.h>
#include <AFMotor.h>

// servo variable
#define SERVO_PIN 9
Servo radarServo;  // create servo object to control a servo
int posArray[] = {10,45,90,135,170};

// ultrasonic variable
#define ECHO_PIN 23
#define TRIG_PIN 24
SR04 ultrasonic = SR04(ECHO_PIN, TRIG_PIN);
int distance[] = {0,0,0,0,0};

// timing variables
long currentTimeSamplePostRadar = 0;
long previousTimeSamplePostRadar = 0;

long currentTimeSampleCalculateRadar = 0;
long previousTimeSampleCalculateRadar = 0;

//perception 
float x = 0;
float y = 0;
float theta = 0;

void setup() {
  Serial2.begin(9600);
  radarServo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object
  delay(1000);
  Serial.println("ultrasonic and servo radar test program");
}

void loop() {
//       Test One
         currentTimeSampleCalculateRadar = millis();
      if(currentTimeSampleCalculateRadar - previousTimeSampleCalculateRadar > 1000){
        previousTimeSampleCalculateRadar = currentTimeSampleCalculateRadar;
         
        }
        currentTimeSamplePostRadar = millis();
      if(currentTimeSamplePostRadar - previousTimeSamplePostRadar > 1000){
        previousTimeSamplePostRadar = currentTimeSamplePostRadar;
          Serial2.print(distance[0]);
          Serial2.print(", ");
          Serial2.print(distance[1]);
          Serial2.print(", ");
          Serial2.print(distance[2]);
          Serial2.print(", ");
          Serial2.print(distance[3]);
          Serial2.print(", ");
          Serial2.println(distance[4]);
        }
      for(int i = 0; i <= 3; i++){    // to go through the array from the begining to the end(excluding)
        radarServo.write(posArray[i]);
        delay(100);
        distance[i] = ultrasonic.DistanceAvg();
        distance[i] > 50 ? distance[i] = 50 : false;  
      }

      for(int i = 4; i >= 1; i--){    // to go through the array from the last element to the begining(excluding)    
        radarServo.write(posArray[i]);
        delay(100);
        distance[i] = ultrasonic.DistanceAvg();
        distance[i] > 50 ? distance[i] = 50 : false;  
      }
      

}
