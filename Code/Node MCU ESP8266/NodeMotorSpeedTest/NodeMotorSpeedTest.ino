

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
String x,y,t,theta,message;

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
  Serial.begin(9600);
  Serial.println("Serial connection with PC is working");
  arduinoSerial.begin(9600);

  WiFi.softAP(ssid,password);
  WiFi.softAPConfig(local_ip, gateway, subnet_mask);
  delay(100);

  server.on("/motorSpeed",handleMotorSpeed);
  server.on("/getSpeedValue",handleMotorSpeedGetValue);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}
void handleMotorSpeed(){  
  server.send(200,"text/html",motor_speed_form);
}

void handleMotorSpeedGetValue(){
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
