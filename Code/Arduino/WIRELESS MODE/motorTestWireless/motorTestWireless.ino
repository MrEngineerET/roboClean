#include <AFMotor.h>

#define LEFT_WHEEL 1
#define RIGHT_WHEEL 2

#define switchPin 22
int mspeed=0;

AF_DCMotor leftWheel(1);
AF_DCMotor rightWheel(4);

void setup() {
 pinMode(switchPin,INPUT_PULLUP);
 Serial2.begin(9600);
 Serial.begin(9600); 
}

void loop() {
  if(Serial2.available() > 0){
      mspeed = Serial2.readStringUntil('\n').toInt();
      Serial.println(mspeed);
    }
  if(digitalRead(switchPin) == 1){
    moveMotor(LEFT_WHEEL,FORWARD,0);
    moveMotor(RIGHT_WHEEL,FORWARD,0);
    delay(1000);
 }else{
    moveMotor(LEFT_WHEEL,FORWARD,mspeed);
    moveMotor(RIGHT_WHEEL,FORWARD,mspeed);
    delay(1000);
 }
}

void moveMotor(int WHEEL,int DIRECTION, int mSpeed){
  if(WHEEL == LEFT_WHEEL){
      if(DIRECTION == FORWARD){
        leftWheel.run(FORWARD);
        leftWheel.setSpeed(mSpeed);
      }else{
        leftWheel.run(BACKWARD);
        leftWheel.setSpeed(mSpeed);
      }
  }else if(WHEEL == RIGHT_WHEEL){
      if(DIRECTION == FORWARD){
        rightWheel.run(FORWARD);
        if(mSpeed == 0){
          rightWheel.setSpeed(mSpeed);
        }else {
          rightWheel.setSpeed(mSpeed+10);
        }
      }else{
        rightWheel.run(BACKWARD);
        if(mSpeed == 0){
          rightWheel.setSpeed(mSpeed);  
        }else{
          rightWheel.setSpeed(mSpeed+10);        
        }
      }
  }
}
