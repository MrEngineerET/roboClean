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

String distances[5];

void setup(){
  Serial.begin(9600);
  arduinoSerial.begin(9600);

  WiFi.softAP(ssid,password);
  WiFi.softAPConfig(local_ip, gateway, subnet_mask);
  delay(100);

  server.on("/radarJSON",handleRadarJSON);
  server.begin();
  Serial.println("HTTP Server Started");
}

void loop(){
   server.handleClient();
    if (arduinoSerial.available()>0){
      String message  = arduinoSerial.readStringUntil('\n');
      parseForDistances(message);
    }  
   delay(1000);
}
void handleRadarJSON(){
  String data = "{\"d0\":"+ distances[0] +",\n";
  data += "\"d2\":"+ distances[1] +",\n";
  data += "\"d2\":"+ distances[2] +",\n";
  data += "\"d3\":"+ distances[3] +",\n";
  data += "\"d4\":"+ distances[4] +",\n";
  server.send(200,"application/json",data);
}


// param 
// numOfValues is the number of values to be extracted from the message 
void parseForDistances(String message){
  short indexes[5]; 
  indexes[0] = message.indexOf(',');
  distances[0] = message.substring(0,indexes[0]);
  
  for(byte i = 1; i < 5; i++){
     indexes[i] = message.indexOf(',',indexes[i-1]+1);
     distances[i] = message.substring(indexes[i-1]+1,indexes[i]);
  }
}
