import processing.serial.*;
Serial myPort;
PFont fnt;
int xPos;
int oldXpos=30;
int oldxtint;
int dx=30;

int xPos2;
int oldXpos2=30;
int oldxtint2;
int dx2=30;

void setup()
{
  
 size(1260,660);
 init_graph();
 xPos=dx;
 xPos2=dx2;
   fnt = createFont("Arial", 16, true);
 myPort=new Serial(this,"COM28",9600);
}
void draw()

{
  
  int ixt=0,n=0;
  int ixt2=0,n2=0;
 //println("aaaa");
  String inString=myPort.readStringUntil('\n');
  if(inString!=null){
  String oldinString=""; 
  if (inString!=oldinString)
  {
    fill(255);
    rect(244,236,100,40);
    fill(0);
    stroke(0,0,0);
     text(inString,250,250);
     oldinString=inString;
  } 
 
 
  String[] motors=splitTokens(inString,",");
  print(inString);
  String[] num=splitTokens(motors[0],".");
   n=Integer.parseInt(num[0]);
   ixt=(n*6); //maybe * up
   println(n);
   stroke(0,0,0);
   smooth();
  line(oldXpos,640-2*oldxtint,xPos,640-2*ixt);
  
  oldXpos=xPos;
  oldxtint=ixt;
  if (xPos >= width-dx)
  { xPos = dx;
    oldXpos=0;  
  }
  else { xPos+=10; }
  
  
  String[] num2=splitTokens(motors[1],".");
   n2=Integer.parseInt(num2[0]);
   ixt2=(n2*6); //maybe * up
   println(n2);
   stroke(250,0,0);
   smooth();
   line(oldXpos2,640-2*oldxtint2,xPos2,640-2*ixt2);
    oldXpos2=xPos2;
    oldxtint2=ixt2;
  if (xPos2 >= width-dx2)
  { xPos2 = dx2;
    oldXpos2=0;  
  }
  else { xPos2+=10; }

  }


}
void init_graph()
{
 background(255,255,255);
//  fill (255);
 stroke(0,0,0);
 line(30,640,1470,640);
 
  
 line(30,640,30,10);
 //textFont(fnt, 14);
   
  textSize(12);
  fill(50);
   
    text ("0", 14, 640);
    text ("2,5", 14, 610);
    text ("5", 14, 580);
    text ("7,5", 14, 550);
    text ("10", 14, 520);
    text ("15", 14, 460);
    text ("20", 14, 400);
    text ("25", 14, 340);
    text ("30", 14, 280);
    text ("35", 14, 220);
    text ("40", 14, 160);
    text ("45", 14, 100);
    text ("50", 14, 40);
    
    text ("_", 30, 640);
    text ("_", 30, 610);
    text ("_", 30, 580);
    text ("_", 30, 550);
    text ("_", 30, 520);
   
    text ("0",30,650);
    text ("3",60,650);
    text ("6",90,650);
    text ("9",120,650);
    text ("12",150,650);
    text ("15",180,650);
    text ("18",210,650);
    text ("21",240,650);
    text ("24",270,650);
    text ("27",300,650);
    text ("30",330,650);
    text ("33",360,650);
    text ("36",390,650);
    text ("39",420,650);
    text ("42",450,650);
    text ("45",480,650);
    text ("48",510,650);
    text ("51",540,650);
    text ("54",570,650);
    text ("57",600,650);
    text ("60",630,650);
    text ("63",660,650);
    text ("66",690,650);
    text ("69",720,650);
    text ("72",750,650);
    text ("75",780,650);
    text ("78",810,650);
    text ("81",840,650);
    text ("84",870,650);
    text ("87",900,650);
    text ("90",930,650);
    text ("93",960,650);
    text ("96",990,650);
    text ("99",1020,650);
    text ("102",1050,650);
    text ("108",1110,650);
    text ("114",1170,650);
    text ("120",1230,650);
    text ("122",1250,650);
    text ("|",30,640);
    text ("|",60,640);
    text ("|",90,640);
    text ("|",120,640);
    text ("|",150,640);
    text ("|",180,640);
    text ("|",210,640);
    text ("|",240,640);
    text ("|",270,640);
    text ("|",300,640);
    text ("|",330,640);
    text ("|",360,640);
    text ("|",390,640);
    text ("|",420,640);
    text ("|",450,640);
    text ("|",480,640);
    text ("|",510,640);
    text ("|",540,640);
    text ("|",570,640);
    text ("|",600,640);
    text ("|",630,640);
    text ("|",660,640);
    text ("|",690,640);
    text ("|",720,640);
    text ("|",750,640);
    text ("|",780,640);
    text ("|",810,640);
    text ("|",840,640);
    text ("|",870,640);
    text ("|",900,640);
    text ("|",930,640);
    text ("|",960,640);
    text ("|",990,640);
    text ("|",1020,640);
    text ("|",1050,640);
    text ("|",1110,640);
    text ("|",1170,640);
    text ("|",1230,640);
    text ("|",1250,640);
    
 textSize(16);
 fill(50);
 text(" [Y]-SPEED (M/MIN) ",21,30);
 text(" [X]-TIME (SEC) ",1080,630);
 
}
   
  