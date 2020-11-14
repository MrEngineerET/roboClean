
Robot myRobot;
Tracing traceRobot;
int backgroundColor = 200;

void setup(){
  size(1020,700);
  myRobot = new Robot(0,0,50);
  traceRobot = new Tracing(myRobot.getX(),myRobot.getY());
  background(backgroundColor);
}

void draw(){
  //fill(0);
  //circle(mouseX,mouseY,50);
  myRobot.move(mouseX,mouseY);
  traceRobot.drawTrace(myRobot.getX(),myRobot.getY());
  
  image(myRobot.graphics(),0,0);
  image(traceRobot.graphics(),0,0);
}

class Robot{
  float x; // the x axis positon for the center of the robot
  float y; // the y axis position for the center of the robot
  float radius; // the radius of the robot;
  PGraphics robotPG;
 
  Robot(float x, float y, int radius){
    this.x = x;
    this.y = y;
    this.radius = radius;
    robotPG = createGraphics(width,height);
  }
  void move(float toX, float toY){
    this.x = toX;
    this.y = toY;
    this.robotPG.beginDraw();
    this.robotPG.background(backgroundColor);
    this.robotPG.fill(0);
    this.robotPG.circle(this.x,this.y,this.radius);
    this.robotPG.endDraw();
    
  }
  float getX(){
    return this.x;
  }
  float getY(){
    return this.y;
  }
  PGraphics graphics(){
    return robotPG; 
  }
}

class Tracing{
  float previousX;
  float previousY;
  PGraphics tracingPG;
  
  Tracing(float previousX, float previousY){
    this.previousX = previousX;
    this.previousY = previousY;
    tracingPG = createGraphics(width,height);
    this.tracingPG.beginDraw();
    this.tracingPG.fill(0);
    this.tracingPG.circle(previousX,previousY,5);
    this.tracingPG.endDraw();
  }
  void drawTrace(float x, float y){
    if(sqrt(sq(x-this.previousX) + sq(y - this.previousY)) > 10){
      this.tracingPG.beginDraw();
      this.tracingPG.fill(0);
      this.tracingPG.circle(previousX,previousY,2);
      this.tracingPG.endDraw();
      this.previousX = x;
      this.previousY = y;
    }
  }
  
    PGraphics graphics(){
    return tracingPG;
  }
}
