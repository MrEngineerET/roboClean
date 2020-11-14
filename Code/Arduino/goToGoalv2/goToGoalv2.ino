// this test program print the velocity and angular velocity of the robot
// median filter for the calculation of velocities is applied

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
const int delta = 100;    // sampling time of the system
unsigned long currentTimeSample = 0;
unsigned long previousTimeSample = 0; 

// ODOMETRY VARIABLES
#define ARRAYSIZE 10
const byte numberOfHole = 20; // number of holes on the motor encoder disk
const float diameter = 6.8; // the radius of left and right wheels[cm]
float Dl = 0; // the latest value of distance moved by the left wheel
float arrDl[ARRAYSIZE]; // array for holding the latest distance moved by the left wheel(Dl) and some other past values
float meanDl = 0; // the mean value of the Dl array
float Dr = 0; // the latest value of distance moved by the right wheel
float arrDr[ARRAYSIZE]; // array for holding the latest distance moved by the right wheel(Dl) and some other past values
float meanDr = 0; // the mean value of the Dr array
float Dc = 0;
float x = 0;  // the x position of the robot
float y = 0;  // the y postition of the robot
float phi = 0; // the orientation of the robot
const float L = 13.4; // length of the robot between tires[cm]

// ROBOT INPUT VARIABLE
float V = 0;  // the overall velocity of the robot [cm/sec]
float Vr = 0; // the velocity of the right wheel
float Vl = 0; // the velocity of the left wheel
float W = 0;  // the angular velocity og the robot

// ROBOT INPUT VARIABLE CONTROLLED BY PID (GO TO GOAL)
float Xg; // the x axis goal position of the robot
float Yg; // the y axis goal position of the robot
float phid = 0; 
float phiErr = 0;
float phiErrOld = 0;
float phiErrSum = 0;
float Kp = 2;
float Ki = 0.05;
float Kd = 0;
const int PWMmax = 100;
const int PWMmin = 35;
const int robotMaxSpeed = 150;  // [cm/s]
const int robotMinSpeed = 30; // [cm/s]

void setup () {
   attachInterrupt (digitalPinToInterrupt(leftEncoderPin), LEncoder, RISING); // interrupt function: interrupt pin, function to call, type of activation
   attachInterrupt (digitalPinToInterrupt(rightEncoderPin), REncoder, RISING); // interrupt function: interrupt pin, function to call, type of activation
   Serial.begin(9600); // start of serial communication
   moveMotor(LEFT_WHEEL,FORWARD,0);
   moveMotor(RIGHT_WHEEL,FORWARD,0);
   delay(5000);
   Xg = 300; 
   Yg = 300;
   V = 60;
}

void loop () {
  currentTimeSample = millis();
  if(currentTimeSample - previousTimeSample > delta){
     previousTimeSample = currentTimeSample;
    
     odometry();
    
     goToGoal();
    
        Serial.print(x); 
        Serial.print(",");
        Serial.print(y);
        Serial.print(","); 
        Serial.print((float)millis()/1000);
        Serial.print(",");
        Serial.println(phi); 

//        
//        Serial.print(phid); 
//        Serial.print(", ");
//        Serial.print(phiErr); 
//        Serial.print(", ");
//        Serial.print(W); 
//        Serial.print(", ");
//        Serial.print(Vl); 
//        Serial.print(", ");
//        Serial.print(Vr); 
//        Serial.print(", ");        
//        Serial.print(leftMotorSpeed); 
//        Serial.print(", ");
//        Serial.print(rightMotorSpeed);
//        Serial.print(", ");        
//        Serial.print(x); 
//        Serial.print(", ");
//        Serial.println(y);
        
      // for observing the angular velocity of the robot
      // Serial.println(W);
    }
}

void LEncoder () {// interrupt function of the left wheel encoder
    if(digitalRead(leftEncoderPin) && (micros()- debounceL > 100) && digitalRead(leftEncoderPin)){
        currentLeftEncoderPulses++;
        debounceL = micros();
    }
}
void REncoder () {// interrupt function of the right wheel encoder
   if(digitalRead(rightEncoderPin) && (micros()- debounceR > 100) && digitalRead(rightEncoderPin)){
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

void odometry(){
  // median filter
  Dl = ((float)(currentLeftEncoderPulses-previousLeftEncoderPulses)/numberOfHole) * PI * diameter;
  previousLeftEncoderPulses = currentLeftEncoderPulses;
  Dr = ((float)(currentRightEncoderPulses-previousRightEncoderPulses)/numberOfHole) * PI * diameter;
  previousRightEncoderPulses = currentRightEncoderPulses;
  byte i = 0;
  for( i; i < ARRAYSIZE - 1; i++){
    arrDl[i] = arrDl[i+1];
    arrDr[i] = arrDr[i+1];
  } 
  arrDl[i] = Dl;
  arrDr[i] = Dr;
  i = 0;
  for( i; i < ARRAYSIZE ; i++){
    meanDl += arrDl[i];
    meanDr += arrDr[i];
  } 
  meanDl /= ARRAYSIZE;
  meanDr /= ARRAYSIZE;
  
  Dc = ((float)Dl+Dr)/2;
  x = x + Dc*cos(phi);
  y = y + Dc*sin(phi);
  phi = phi + (Dr - Dl)/L;
  phi = atan2(sin(phi),cos(phi));
}

void goToGoal(){
   if(abs(Xg - x) < 10 && abs(Yg - y) < 10){
      moveMotor(LEFT_WHEEL,FORWARD,0);
      moveMotor(RIGHT_WHEEL,FORWARD,0);
      return;
   }
  phid = atan2(Yg-y,Xg-x);
  phiErr = phid - phi;
  phiErrSum += phiErr;
  if(phiErr < PI/8 && phiErr > -PI/8){
    smallPhiErrorController();
  }else{
    largePhiErrorController();
  }
}
float smallPhiErrorController(){ // for phiError < pi/8 or phiError > -phi/8
  // PID controller for angular velocity of the robot
  W = Kp * phiErr + Ki*phiErrSum + Kd*((phiErr-phiErrOld)/delta);
  phiErrOld = phiErr;
  // unicycle( v,w) to differential (Vl,Vr)
  Vl = (2*V - W*L)/2;
  Vr = (2*V + W*L)/2;
  leftMotorSpeed = map(Vl,robotMinSpeed,robotMaxSpeed,PWMmin,PWMmax);
  rightMotorSpeed = map(Vr,robotMinSpeed,robotMaxSpeed,PWMmin,PWMmax);
  moveMotor(LEFT_WHEEL,FORWARD,leftMotorSpeed);
  moveMotor(RIGHT_WHEEL,FORWARD,rightMotorSpeed);
}

float largePhiErrorController(){ // for phiError > pi/8 or phiError < -phi/8
  if(phiErr >= PI/8){
    // move the robot in counter clockwise  
     moveMotor(LEFT_WHEEL,FORWARD,0);
     moveMotor(RIGHT_WHEEL,FORWARD,PWMmin);
  }else if(phiErr <=-PI/8){
    // move the robot in clockwise
     moveMotor(LEFT_WHEEL,FORWARD,PWMmin);
     moveMotor(RIGHT_WHEEL,FORWARD,0);
  }
    phiErrSum = 0;
    phiErrOld = 0;
}
