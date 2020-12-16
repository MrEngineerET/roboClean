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

String distances[5],message;

const char homePage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Home Page</title>
</head>
<body>
    <nav>
        <a href="#">Home Page</a>
        <a href="about">About</a>
        <a href="/control">Control</a>
    </nav>

    <div id="top">
        <h1>Robo Clean</h1>
        <p>
            <form action="/SpeedValue", method="post">
            <span>Set PWM: </span> 
            <input type="number" step="1", name="motorSpeed" id="text">
            <input type="submit" value="Run" id="run">
        </form>
        </p>
    </div>
    <div id="bottom">
        <p class="c">Controls</p>
            <div id="buttons">
              <a href="/upButton"><button>Up</button></a><br>
              <a href="/leftButton"><button>Left</button></a>
              <a href="/rightButton"><button>Right</button></a><br><br><br>
              <a href="/downButton"><button>Down</button></a>
            </div><br>
        <p class="mimenan">V : 34m/s</p>
        <p class="mimenan1">w : 5rad/s</p>
        <p id="upButtonData"></p>
    </div>
    <style>
        body{
            width: 320px;
            height: 600px;
            margin:auto;
            font-family: Arial, Helvetica, sans-serif;
            background-color:rgb(225, 237, 238) ;
        }
        #top{
            margin-top: 100px;
            width: 300px;
            height:120px;
            background-color:white;
            padding: 15px;
            border-radius: 15px;
            box-shadow: 5px 5px 7px gray;
        }
        h1{
            font-size: 30px;
            text-align: center;
            color: hsl(179, 72%, 30%);
        }
        p{
            text-align: center;
        }
        .c, .mimenan,.mimenan1{
            color: white;
            font-size: 20px;
            font-weight: bolder;
        }
        #bottom{
            width: 300px;
            height: 300px;
            margin-top: 15px;
            margin-left: auto;
            margin-right: auto;
            padding: 15px;
            border-radius: 15px;
            text-align: center;
            background-color: rgb(26, 136, 139);
            box-shadow: 3px 3px 10px black;
        }
        #run{
            width: 50px;
            height: 25px;
            border-radius: 10px;
            background-color: hsl(179, 62%, 43%);
            color: rgb(225, 237, 238)
        }
        #up, #left, #right, #down{
            width: 100px;
            height: 50px;
            border-radius: 15px;
            background-color: hsl(187, 73%, 54%);
            color: white;
            font-weight: bold;
        }
        #up{
            align-items: center;
        }
        #left{
            float: left;
            clear: left;
        }
        #right {
            float: right;
        }
        #down{
            align-items: center;
            
        }
        input#text{
            border-radius: 5px;
        }
        span{
            font-weight: bold;
        }
        #buttons{
            margin-top: 20px;
        }
        button{
            box-shadow: 3px 3px 10px  hsl(178, 90%, 8%);
        }
        .mimenan, .mimenan1{
            margin: 1px;
            float: left;
            clear: left;
            font-size: 20px;
            font-weight: bold;           
        }
    </style>
</body>
</html>
</body>
</html>
)rawliteral";
const char aboutPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>About Page</title>
</head>
<body>
    <nav>
        <li><a href="/home">Home Page</a></li>
        <li><a href="#">About</a></li>
        <li><a href="/control">Control</a></li>
    </nav>
</body>
</html>
)rawliteral";
const char controlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Control Page</title>
</head>
<body>
    <nav>
        <li><a href="/home">Home Page</a></li>
        <li><a href="/about">About</a></li>
        <li><a href="#">Control</a></li>
    </nav>
</body>
</html>
)rawliteral";

void setup(){
  Serial.begin(9600);
  arduinoSerial.begin(9600);

  WiFi.softAP(ssid,password);
  WiFi.softAPConfig(local_ip, gateway, subnet_mask);
  delay(100);

  server.on("/",handleHomePage);
  server.on("/SpeedValue",handlePostSpeedValue);
  server.on("/about",handleAboutPage);
  server.on("/control",handleControlPage);
  server.on("/upButton",handleUpButton);
  server.on("/leftButton",handleLeftButton);
  server.on("/rightButton",handleRightButton);
  server.on("/downButton",handleDownButton);
  server.begin();
  Serial.println("HTTP Server Started");
}

void loop(){
   server.handleClient();
   delay(10);
}


void handleUpButton(){
  arduinoSerial.println("01");
  server.send(200,"text/html",homePage);
}

void handleLeftButton(){
   arduinoSerial.println("03");
  server.send(200,"text/html",homePage);
}

void handleRightButton(){
   arduinoSerial.println("04");
  server.send(200,"text/html",homePage);
}

void handleDownButton(){
  arduinoSerial.println("02");
  server.send(200,"text/html",homePage);
}


void handlePostSpeedValue(){
   if(server.method() == HTTP_POST){
    for (uint8_t i = 0; i < server.args(); i++) {
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
      if(server.argName(i) == "speedValue"){
//        arduinoSerial.println(server.arg(i));
      }else if(server.argName(i) == "stop"){
        arduinoSerial.println("0");  
        Serial.println("stop is pressed");
      }
    }
   Serial.println(message);
//   server.send(200,"text/html",motor_speed_form);
  }  
}

void handleHomePage(){
  server.send(200,"text/html",homePage);
}
void handleAboutPage(){
  server.send(200,"text/html",aboutPage);
}
void handleControlPage(){
  server.send(200,"text/html",controlPage);
}
