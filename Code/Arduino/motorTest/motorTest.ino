#include <AFMotor.h>

#define LEFT_WHEEL 1
#define RIGHT_WHEEL 2

#define switchPin 22
int mspeed=0;

AF_DCMotor leftWheel(1);
AF_DCMotor rightWheel(4);

void setup() {
 Serial.begin(9600);
 Serial.println("Enter a number between 0 and 255 to move the motor");
 pinMode(switchPin,INPUT_PULLUP);
      
}

void loop() {
  if(Serial.available() > 0){
      mspeed = Serial.readString().toInt();
      delay(1000);
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
        rightWheel.setSpeed(mSpeed);
      }else{
        rightWheel.run(BACKWARD);
        rightWheel.setSpeed(mSpeed);
      }
  }
}
