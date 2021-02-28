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
String x,y,t,theta ;

const char homePage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
    <head><title>Home page</title></head>
    <body>
        <div id="navigation">
            <p>
                <span class="nav"><a href="/manual">Manual</a></span>
                <span class="nav"><a href="/config">Configuration</a></span>
                <span class="nav"><a href="/test">Test</a></span>
                <span class="nav"><a href="#">About </a></span>
            </p>
        </div>
        <div id="buttons">
            <span class="buttons">
                <a href="/startAuto"><button>Start</button></a>
                <a href="/stopAuto"><button>Stop</button></a>
            </span>
        </div>
        <style>
            body{
                font-family: Arial, Helvetica, sans-serif;
                width: 350px;
                height: 150px;
                margin-left: auto;
                margin-right: auto;
                margin-top: auto;
                background-color: rgb(165, 165, 165);
            }
            #navigation{
                width:350px;
                height:50px;
                margin-right: auto;
                margin-left: auto;
                box-shadow: 10px 10px 6px rgb(75, 73, 73);
            }
            a{
                text-decoration: none;
                color: rgb(78, 74, 74) ;
            }
            div{
                width: 320px;
                height: 120px;
                margin: 15px;
                padding: 15px;
                background-color: white;
                border-radius: 15px;
                box-shadow: 10px 10px 6px rgb(75, 73, 73);
            }
            button{
                width: 130px;
                height: 50px;
                margin-left: 5px;
                margin-right: 5px;
                border-radius: 5px;
                background-color: rgb(18, 131, 103);
                color:white;
                font-weight: bold;
            }
            .buttons{
                position: relative;
                top: 35px;
                margin: 20px;
                margin-right: auto;
                margin-top: 70px;
                
            }
            .nav{
                margin-left: 16px;
                margin-right: 10px;
            }
        </style>

    </body>
</html>
)rawliteral";
const char manualPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <title>Robo Clean</title>
    </head>
    <body>
        <div id="navigation">
            <p id="navs"><span class="nav"><a href="/">Home</a></span>
                <span class="nav"><a href="/config">Config</a></span>
                <span class="nav"><a href="/test">Test</a></span>
                <span class="nav"><a href="#">About </a>> </span>
            </p>
        </div>
        <div id="top">
            <h1>Robo Clean</h1>
            <p>
                <form action="/speedValue", method="post">
                <span>Set PWM: </span> 
                <input type="number" step="1", name="motorSpeed" id="text">
                <input type="submit" value="Run" id="run">
            </form>
            </p>
        </div>
        <div id="bottom">
            <p class="c">Controls</p>
            <div id="buttons">
                <a href="forwardButton"><button id="up">Up</button></a><br>
                <a href="leftButton"><button id="left">Left</button></a>
                <a href="rightButton"><button id="right">Right</button></a><br><br><br>
                <a href="backwardButton"><button id="down">Down</button></a>
            </div>
            <br><a href="stopButton"><button id="Stop">Stop</button></a></p>
        </div>
        <style>
            body{
                width: 320px;
                height: 600px;
                margin:auto;
                font-family: Arial, Helvetica, sans-serif;
                background-color:rgb(165, 165, 165) ;
            }
            #top{
                margin-top: 10px;
                width: 300px;
                height:120px;
                background-color:white;
                padding: 15px;
                border-radius: 15px;
                box-shadow: 5px 5px 7px gray;
                margin-left: auto;
                margin-right: auto;
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
            #up, #left, #right, #down, #Stop{
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
            #navigation{
                position: relative;
                background-color: white;
                width:350px;
                height:70px;
                box-shadow: 5px 5px 7px gray;
                border-radius: 10px;
                margin-top:10px;
                margin-left: auto;
                right: 10px;
         
                
            }
            #navs{
                font-weight: bolder;
                position: relative;
                top: 25px;
            }
            .nav{
                margin-left: 15px;
                margin-right: 15px;
            }
            a{
                text-decoration: none;
                color: rgb(78, 74, 74) ;
            }
            
        </style>
    </body>
</html>
)rawliteral";

const char testPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <title>Test</title>
    </head>
    <body>
        <div id="navigation">
            <p id="navs"><span class="nav"><a href="/">Home</a></span>
                <span class="nav"><a href="/manual">Manual</a></span>
                <span class="nav"><a href="/config">Config</a></span>
                <span class="nav"><a href="#">About </a> </span>
            </p>
        </div>
        <div id="main">
                <p class="motor">Motor <a href="motorTest"><button>Test</button> RW<a href="randomWalk"><button>Test</button></a></a></p>
             
        </div>
        <style>
             body{
                font-family: Arial, Helvetica, sans-serif;
                width: 350px;
                height: 150px;
                margin-left: auto;
                margin-right: auto;
                margin-top: auto;
                background-color: rgb(165, 165, 165);
            }    
            #navigation{
                background-color: white;
                width:350px;
                height:70px;
                box-shadow: 10px 10px 6px rgb(75, 73, 73);
                border-radius: 10px;
                margin: 10px;
                margin-right: auto;
                margin-left: auto;
            }
            #navs{
                position: relative;
                top: 25px;
                margin-left: auto;
                margin-right: auto;
            }
            .nav{
                margin-left: 15px;
                margin-right: 15px;
            }
            a{
                text-decoration: none;
                color: rgb(78, 74, 74) ;
            }
            button{
                width: 90px;
                height: 30px;
                margin-left: 5px;
                margin-right: 5px;
                border-radius: 5px;
                background-color: rgb(18, 131, 103);
                color:white;
                font-weight: bold;
            }
            #main{
                width: 350px;
                height: 100px;
                margin-left: auto;
                margin-right: 5px;
                border-radius: 10px;
                box-shadow: 10px 10px 6px rgb(75, 73, 73);
                background-color: white;
                font-weight: bold;
            }
            .motor{
                position: relative;
                top: 35px;
                left: 85px;
                margin-left: auto;
                margin-right: auto;
            }
        </style>
    </body>
</html>
)rawliteral";

const char configPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
    <head><title>Configuration</title></head>
    <body>
        <div id="navigation">
            <p id="navs"><span class="nav"><a href="/">Home</a></span>
                <span class="nav"><a href="/manual">Manual</a></span>
                <span class="nav"><a href="/test">Test</a></span>
                <span class="nav"><a href="#">About </a> </span>
            </p>
        </div>

        <div id="parameters">
            <form action="/configrationValues",method="post">
                <fieldset>
                    <legend>PID Parameters</legend>
                    <label>Kp: <br />
                        <input type="text" name="Kp" /><br />
                    </label>
                    <label>Ki: <br />
                        <input type="text" name="Ki" /></br>
                    </label>
                    <label>Kd: <br />
                        <input type="text" name="kd" /><br />
                    </label>
                    <button class="send" type= "submit">Send</button>
                    <p><br />Straight line test</p>
                    <a href="/goStraightLine"><button class="go">Go</button></a>
                </fieldset>
            </form>
        </div>
        <style>
            body{
                margin-left: auto;
                margin-right: auto;
                width: 450px;
                height: 300px;
                font-family: Arial, Helvetica, sans-serif;
                background-color: rgb(165, 165, 165);
            }
            #navigation{
                background-color: white;
                width:430px;
                height:70px;
                box-shadow: 10px 10px 6px rgb(75, 73, 73);
                border-radius: 10px;
                margin: 10px;
                margin-right: auto;
                margin-left: auto;
            }
            #navs{
                position: relative;
                top: 25px;
                margin-left: auto;
                margin-right: auto;
            }
            .nav{
                margin-left: 30px;
                margin-right: 30px;
            }
            a{
                text-decoration: none;
                color: rgb(78, 74, 74) ;
            }
            #parameters{
                width:430px;
                height:330px ;
                border-radius: 10px;
                box-shadow: 10px 10px 6px rgb(75, 73, 73);
                background-color: white;
                margin-left:auto ;
                margin-right: auto;
            }
            fieldset{
                position: relative;
                top: 16px;
                padding-top: 15px;
                margin-left: auto;
                margin-right: auto;
                width: 350px;
                height: 265px;
            }
            .send{
                margin-top: 5px;
            }
            button{
                width: 70px;
                height: 30px;
                border-radius: 5px;
                background-color: rgb(18, 131, 103);
                color:white;
                font-weight: bold;
            }
            .go{
                position: relative;
                top: -12px;
            }
        </style>
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
  server.on("/speedValue",handlePostSpeedValue);
  server.on("/config",handleConfigPage);
  server.on("/manual",handleManualPage);
  server.on("/test",handleTestPage);
  server.on("/forwardButton",handleForwardButton);
  server.on("/leftButton",handleLeftButton);
  server.on("/rightButton",handleRightButton);
  server.on("/backwardButton",handleBackwardButton);
  server.on("/stopButton",handleStopButton);
  server.on("/startAuto",handleStartAuto);
  server.on("/stopAuto",handleStopAuto);
  server.on("/motorTest",handleMotorTest);
  server.on("/configrationValues",handlePostConfigrationValue);
  server.on("/goStraightLine",handleGoStraitLine);
  server.on("/randomWalk",handleRandomWalk);
  server.on("/pose",handleMatlabJSON);   
  server.begin();
  Serial.println("HTTP Server Started");

  x = y = t = theta = "0.0";
}

void loop(){
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

void handlePostConfigrationValue(){
  String Kp, Ki,  Kd = "0";
  String message = "10,";
  for(uint8_t i = 0; i < server.args(); i++){
    if(server.argName(i) == "Kp"){
      Kp = server.arg(i);
      message+=Kp;
      message+=",";
    }else if(server.argName(i) == "Ki"){
      Ki = server.arg(i);
      message+=Ki;
      message+=",";
    }else if(server.argName(i) == "Kd"){
      Kd = server.arg(i);
      message+=Kd;
      message+=",";
    }
  }
  arduinoSerial.println(message);
  server.send(200,"text/html",configPage);
}

void handleRandomWalk(){
  arduinoSerial.println("33");
  server.send(200,"text/html",testPage);
}

void handleGoStraitLine(){
  arduinoSerial.println("08");
  server.send(200,"text/html",configPage);
}

void handleStartAuto(){
 arduinoSerial.println("06");
 server.send(200,"text/html",homePage);
}
void handleStopAuto(){
  arduinoSerial.println("07");
  server.send(200,"text/html",homePage);
}

void handleMotorTest(){
  arduinoSerial.println("09");
  server.send(200,"text/html",testPage);
}

void handleStopButton(){
  arduinoSerial.println("05");
  server.send(200,"text/html",manualPage);
}

void handleForwardButton(){
  arduinoSerial.println("22");
  server.send(200,"text/html",manualPage);
}

void handleLeftButton(){
   arduinoSerial.println("03");
  server.send(200,"text/html",manualPage);
}

void handleRightButton(){
   arduinoSerial.println("04");
  server.send(200,"text/html",manualPage);
}

void handleBackwardButton(){
  arduinoSerial.println("02");
  server.send(200,"text/html",manualPage);
}


void handlePostSpeedValue(){
   if(server.method() == HTTP_POST){
    for (uint8_t i = 0; i < server.args(); i++) {
        if(server.argName(i) == "speedValue"){
        String message = "11";
        message += ",";
        message += server.arg(i);
        message += ",";
        arduinoSerial.println(message);
      }
    }
   server.send(200,"text/html",manualPage);
  }  
}

void handleHomePage(){
  server.send(200,"text/html",homePage);
}

void handleConfigPage(){
  server.send(200,"text/html",configPage);
}
void handleManualPage(){
  server.send(200,"text/html",manualPage);
}
void handleTestPage(){
  server.send(200,"text/html",testPage);
}
