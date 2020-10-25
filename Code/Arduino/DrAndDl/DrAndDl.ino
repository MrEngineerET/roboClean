// this test program print distance moved by left and right wheel for delta(100ms)

#include <math.h> // required to use atan () function
#define PI 3.1415926535897932384626433832795 // PI number definition
#include <AFMotor.h>

// MOTOR VARIABLES
#define LEFT_WHEEL 1
#define RIGHT_WHEEL 2

//LEFT WHEEL VARIABLES
int leftEncoderPin = 18; // Pin 18, where the left encoder pin DO is connected
volatile unsigned long currentLeftEncoderPulses = 0;   // Number of left Encoder pulses
volatile unsigned long previousLeftEncoderPulses = 0;   // Number of left Encoder pulses
int leftMotorSpeed = 0;   // speed value for leftMotor which is between 0 and 255
AF_DCMotor leftWheel(3); // Motor 3 section of the motor shield will be used for left Motor of the robot

//RIGHT WHEEL VARIABLES
int rightEncoderPin = 19; // Pin 19, where the right ecoder pin DO is connected
volatile unsigned long currentRightEncoderPulses = 0;  // Number of right Encoder pulses
volatile unsigned long previousRightEncoderPulses = 0;  // Number of right Encoder pulses
int rightMotorSpeed = 0;    // speed value for rightMotor which is between 0 and 255
AF_DCMotor rightWheel(4); // Motor 4 section of the motor shield will be used for right Motor of the robot

// variable used for reading a clean and good signal from the encoder
volatile unsigned long debounceL = 0;   // time stamp of the last bounce of the incoming signal from the left encoder
volatile unsigned long debounceR = 0;   // time stamp of the last bounce of the incoming signal from the right encoder

// DESIGN VARIABLE
const byte delta = 100;    // sampling time of the system
unsigned long currentTimeSample = 0;
unsigned long previousTimeSample = 0; 

// DISTANCE AND VELOCITY MEASUREMENT VARIABLES
const byte numberOfHole = 20; // number of holes on the motor encoder disk
const float diameter = 6.8; // the radius of left and right wheels[cm]
unsigned long Dl = 0;
unsigned long Dr = 0;
int mspeed = 0;

void setup () {
   attachInterrupt (digitalPinToInterrupt(leftEncoderPin), LEncoder, RISING); // interrupt function: interrupt pin, function to call, type of activation
   attachInterrupt (digitalPinToInterrupt(rightEncoderPin), REncoder, RISING); // interrupt function: interrupt pin, function to call, type of activation
   Serial.begin(9600); // start of serial communication
   moveMotor(LEFT_WHEEL,FORWARD,60);
   moveMotor(RIGHT_WHEEL,FORWARD,60);
}

void loop () {
  currentTimeSample = millis();
  if(currentTimeSample - previousTimeSample > delta){
      previousTimeSample = currentTimeSample;
    // calculate the distance traveled by both left and right wheel
      Dl = ((float)(currentLeftEncoderPulses-previousLeftEncoderPulses)/numberOfHole) * PI * diameter;
      Dr = ((float)(currentRightEncoderPulses-previousRightEncoderPulses)/numberOfHole) * PI * diameter;

      previousLeftEncoderPulses = currentLeftEncoderPulses;
      previousRightEncoderPulses = currentRightEncoderPulses;
      
      Serial.print(Dr); 
      Serial.print(" ");
      Serial.println(Dl);
    }
}

void LEncoder () {// interrupt function of the left wheel encoder
    if(micros()-debounceL > 500){
        currentLeftEncoderPulses++;
        debounceL = micros();
    }
}
void REncoder () {// interrupt function of the right wheel encoder
   if(micros()-debounceR > 500){
        currentRightEncoderPulses++;
        debounceR = micros();
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
