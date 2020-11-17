

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600);
  delay(2000);
  Serial.println("Message is about to send to Node MCU");
}

void loop() {
  Serial2.println("message from arduino");
  delay(3000);
  if(Serial2.available()>0){
    Serial.println(Serial2.readString());
    }
}
