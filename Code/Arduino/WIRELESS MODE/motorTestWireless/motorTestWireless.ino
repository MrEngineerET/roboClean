#include <AFMotor.h>

#define LEFT_WHEEL 1
#define RIGHT_WHEEL 2

#define switchPin 22
int mspeed=0;

AF_DCMotor leftWheel(3);
AF_DCMotor rightWheel(4);

void setup() {
 pinMode(switchPin,INPUT_PULLUP);
 Serial2.begin(9600);
      
}

void loop() {
  if(Serial2.available() > 0){
      mspeed = Serial2.readStringUntil('\n').toInt();
    }
  if(digitalRead(switchPin) == 1){
    moveMotor(LEFT_WHEEL,FORWARD,0);
    moveMotor(RIGHT_WHEEL,FORWARD,0);
    delay(3000);
 }else{
    moveMotor(LEFT_WHEEL,FORWARD,mspeed);
    moveMotor(RIGHT_WHEEL,FORWARD,mspeed);
    delay(3000);
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
        rightWheel.setSpeed(mSpeed);
      }else{
        rightWheel.run(BACKWARD);
        rightWheel.setSpeed(mSpeed);
      }
  }
}
