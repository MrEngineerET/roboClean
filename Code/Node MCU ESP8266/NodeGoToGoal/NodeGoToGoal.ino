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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  arduinoSerial.begin(9600);

  WiFi.softAP(ssid,password);
  WiFi.softAPConfig(local_ip, gateway, subnet_mask);
  delay(100);

  server.on("/",handleHomePage);
  server.on("/pose", handleMatlabJSON);
  server.begin();
  Serial.println("HTTP Server Started");
  x = y = t = theta = "0.0";

}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  if (arduinoSerial.available()>0){
      message  = arduinoSerial.readStringUntil('\n');
      int ind1 = message.indexOf(',');
      x = message.substring(0,ind1);
      int ind2 = message.indexOf(',',ind1+1);
      y = message.substring(ind1+1,ind2);
      int ind3 = message.indexOf(',',ind2+1);
      t = message.substring(ind2+1,ind3);
      int ind4 = message.indexOf(',',ind3+1);
      theta = message.substring(ind3+1,ind4);
  }
}


void handleMatlabJSON(){
  String ptr = "{\"x\":"+ x +",\n";
  ptr += "\"y\":"+ y +",\n";
  ptr += "\"time\":"+ t +",\n";
  ptr += "\"theta\":"+ theta +"}"; 
  server.send(200,"application/json",ptr);
}

void handleHomePage(){
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<html lang=\"en\">\n";
  ptr += "<head>\n";
  ptr += "<meta charset=\"UTF-8\">\n";
  ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  ptr += "<title>Home Page</title>\n";
  ptr += "<style>\n";
  ptr += "html { \n";
  ptr += "font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #1abc9c;}\n";
  ptr += ".button-on:active {background-color: #16a085;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Robo Clean</h1>\n";
  ptr += "<h2> Position of the robot </h2>\n";
  ptr += "<p> position X :"+String(x)+" \n";
  ptr += "<br>\n";
  ptr += "position Y :"+String(y)+" \n"; 
  ptr += "<br>\n";
  ptr += "theta       :"+String(theta)+"\n";    
  ptr += "</p>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  
  server.send(200,"text/html",ptr);
}
