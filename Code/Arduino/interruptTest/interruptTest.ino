// test interrupt pins with arduino

int leftEncoder = 18; // Pin 18, where the left encoder pin DO is connected
int rightEncoder = 19; // Pin 19, where the right ecoder pin DO is connected

volatile int leftEncoderPulses = 0;   // Number of left Encoder pulses
volatile int rightEncoderPulses = 0;  // Number of right Encoder pulses

// variable used for reading a clean and good signal from the encoder
volatile unsigned long debounceL = 0;   // time stamp of the last bounce of the incoming signal from the left encoder
volatile unsigned long debounceR = 0;   // time stamp of the last bounce of the incoming signal from the right encoder

unsigned long timeOld = 0;    // time stamp of the last serial output

void setup () {
   attachInterrupt (digitalPinToInterrupt(leftEncoder), LEncoder, RISING); // interrupt function: interrupt pin, function to call, type of activation
   attachInterrupt (digitalPinToInterrupt(rightEncoder), REncoder, RISING); // interrupt function: interrupt pin, function to call, type of activation
   Serial.begin (9600); // start of serial communication
   Serial.println ("Hello From Encoder");

}

void loop () {
  if(millis() - timeOld > 3000){
      timeOld = millis();
      Serial.print("Left Count: ");
      Serial.println(leftEncoderPulses);  
      Serial.print("Right Count: ");
      Serial.println(rightEncoderPulses);  
    }
}

void LEncoder () {// interrupt function of the left wheel encoder
    if(digitalRead(leftEncoder) && (micros()- debounceL > 50) && digitalRead(leftEncoder)){
        leftEncoderPulses++;
        debounceL = micros();
    }
}
void REncoder () {// interrupt function of the right wheel encoder
   if(digitalRead(rightEncoder) && (micros()- debounceR > 50) && digitalRead(rightEncoder)){
        rightEncoderPulses++;
        debounceR = micros();
    }
}
