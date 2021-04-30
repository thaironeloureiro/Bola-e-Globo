import processing.serial.*;
import cc.arduino.*;
Arduino arduino;

int xMin = 1024;
int yMin = 1024;
int zMin = 1024;

int xMax = 0;
int yMax = 0;
int zMax = 0;
int prevX = 0;
int prevY = 0;

int 
X,Y;
final int
TEXT_COL      = 0xaa000000,
DATA_COL      = 0x99ff0000,
HOVER_COL     = 0xffff0000,
WORLD_TINT    = 0xffffffff,
LINES_WEIGHT  = 2,
BUFF_LINES_W  = 6;
float 
a,b;
PGraphics 
bg, 
hover;
PFont 
h0,h1,h2,h3;
//
Globe w;
Table t;
DataHolder[] data;

////////////////////////////////////////////////////////////////////////////////////////
int cc =0;
void setup(){
  println(Arduino.list());
  println("test: "+cc);
  
  if(cc==0)
  arduino = new Arduino(this, Arduino.list()[1], 57600);
  cc++;
  //Buffers
  size(900,700,P3D); 
  bg= createGraphics(width,height, P2D);    //Buffer for storing the background
  hover=  createGraphics(width,height,P3D); //Color picking buffer
   //Fonts
  h1= loadFont("Lato-Regular-24.vlw");
  h2= loadFont("Lato-Light-24.vlw");
   //General settings
  X=  width/2+100;
  Y= height/2;
  createBackground (bg,X,Y,.1);
  frameRate(30);
  cursor(CROSS);
  //textMode(SCREEN);
   //Objects
  w= new Globe(250,24,"w.png");
  t= new Table("coords.csv");
  data= new DataHolder[t.getNumRows()-1];
  for(int i=0;i<data.length;i++) data[i]= new DataHolder(i);   
}
  
  void createBackground (PGraphics pg, int X, int Y,float f){ 
      int x,y;
      pg.beginDraw();
      pg.smooth();
      for(int i=0;++i<pg.width*pg.height;) 
       {pg.set (x=i%pg.width, y=i/pg.width, (255-round(dist(x,y,X,Y)*f))*0x010101);}
      pg.endDraw();
      background(pg);
  }

////////////////////////////////////////////////////////////////////////////////////////

void draw(){
  getDados();
  background(bg);
  hover.beginDraw(); hover.background(0); hover.endDraw();
  lights();  
  w.update();
  render(X,Y); 
  detectHover();
}

void render(int x, int y){
  hover.beginDraw();
  pushMatrix();
    translate(x,y);
    hover.translate(x,y);
    pushMatrix();
      rotateX(w.rotation.x);
      rotateY(w.rotation.y);
      hover.rotateX(w.rotation.x);
      hover.rotateY(w.rotation.y);
      fill(WORLD_TINT);
      w.render();
      for (int i=0;i<data.length;i++){
          data[i].render(g,false);
          data[i].render(hover,true);
      }
    popMatrix();
  popMatrix();
  hover.endDraw();
}

////////////////////////////////////////////////////////////////////////////////////////
void getDados () {
  int xValue;
  int yValue;

    xValue = arduino.analogRead(0);
    println ("xValue: "+ xValue);
  /*   if (xValue > xMax) xMax = xValue;
     if (xValue < xMin) xMin = xValue;
     xValue = int(map(xValue,xMin,xMax,-90,90));*/
     xValue = int(map(xValue,0,255,-90,90));
     
     yValue = arduino.analogRead(1);
     
  /*   if (yValue > yMax) yMax = yValue;
     if (yValue < yMin) yMin = yValue;
     yValue = int(map(yValue,xMin,xMax,-90,90));*/
     yValue = int(map(yValue,0,255,-90,90));
     
     //if (abs(xValue) > 6 || abs(yValue) > 6) {        
       if(prevX==0 && prevY==0)
       {
         prevX=xValue;
        prevY=yValue; 
       }
       
//       xValue = (int) lerp (prevX, xValue, .1);
//       yValue = (int) lerp (prevY, yValue, .1);
       println ("prevX - xValue: "+prevX +" "+ xValue);
       w.addRotation(xValue,yValue,prevX,prevY);
       
       if (frameCount%80 == 0 ) {
         prevX=xValue;
         prevY=yValue;
       }
     //}           
}  

void mousePressed() {

}
void mouseDragged(){
  if (mouseButton==LEFT)  w.addRotation(mouseX,mouseY,pmouseX,pmouseY);
}


void detectHover(){
  int c=hover.get(mouseX,mouseY);
  int index= c/0x010101 + 254; 
  for(int i=0;i<data.length;i++){
    if (i==index) {
      data[i].setHoveredTo(true);
      fill(TEXT_COL);
      textFont(h1);
      text(data[i].NAME+", "+data[i].COUNTRY,75,height-175);
      textFont(h2);
      text("População: "+nfc(data[i].VALUE),75,height-150);
      noFill();
    }else{
      data[i].setHoveredTo(false);}
  }
}

