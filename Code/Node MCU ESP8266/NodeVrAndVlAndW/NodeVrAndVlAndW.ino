

#include  <ESP8266WiFi.h>
#include  <ESP8266WebServer.h>

#include<SoftwareSerial.h>

SoftwareSerial arduinoSerial(D1,D2);

const char* ssid = "RoboClean";
const char* password = "qazwsxedc";

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet_mask(255,255,255,0);

ESP8266WebServer server(80);

// variables for data send from arduino
String Vr,Vl,V,w,t,message;

const char motor_speed_form[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Testing the motor of speeds</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <h2>Robo Clean Speed Test</h2> 
  <form action="/getSpeedValue", method = "post">
    Speed in cm/s <input type="number" step="1" name="motorSpeed">
    <input type="submit" value="RUN">
  </form>
</body></html>)rawliteral";


void setup() {
  // put your setup code here, to run once:
  Serial.println("Serial connection with PC is working");
  arduinoSerial.begin(9600);

  WiFi.softAP(ssid,password);
  WiFi.softAPConfig(local_ip, gateway, subnet_mask);
  delay(100);

  server.on("/motorSpeed",handleMotorTest);
  server.on("/getSpeedValue",handleMotorTestGetValue);
  // endpoint for matlab and processing to get all the speed value and angular velocity
  server.on("/velocities",handleGetVelocity);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  if(arduinoSerial.available()>0){
     message  = arduinoSerial.readStringUntil('\n');
      int ind1 = message.indexOf(',');
      Vr = message.substring(0,ind1);
      int ind2 = message.indexOf(',',ind1+1);
      Vl = message.substring(ind1+1,ind2);
      int ind3 = message.indexOf(',',ind2+1);
      V = message.substring(ind2+1,ind3);
      int ind4 = message.indexOf(',',ind3+1);
      w = message.substring(ind3+1,ind4);
      int ind5 = message.indexOf(',',ind4+1);
      t = message.substring(ind4+1,ind5);
  }
}
void handleGetVelocity(){
  String ptr = "{\"Vr\":"+ Vr +",\n";
  ptr += "\"Vl\":"+ Vl +",\n";
  ptr += "\"V\":"+ V +",\n";
  ptr += "\"w\":"+ w +",\n";
  ptr += "\"t\":"+ t +"}"; 
  server.send(200,"application/json",ptr);
}

void handleMotorTest(){  
  server.send(200,"text/html",motor_speed_form);
}

void handleMotorTestGetValue(){
  if(server.method() == HTTP_POST){
    
    for (uint8_t i = 0; i < server.args(); i++) {
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
      if(server.argName(i) == "motorSpeed"){
        arduinoSerial.println(server.arg(i));
      }
    }
    Serial.println(message);
    server.send(200, "text/plain", message);  
  }
}
