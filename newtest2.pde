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

void mousePressed() {
 init_graph();
}



void init_graph() {
  background(255, 255, 255);
  stroke(0);
  line(30, 640, 1470, 640);
  line(30, 640, 30, 10);
  textSize(12);
  fill(50);
  for ( int i=0; i<11; i++ ) { 
    text( ( i*5), 30, 640-60*i );
  }
  for ( int i=0; i<42; i++ ) { 
    text( ( i*3), 30+30*i, 650 ); 
    text ("|", 30+30*i, 640);
  }
  textSize(16);
  text(" [Y]-SPEED (M/MIN) ", 21, 30);
  text(" [X]-TIME (SEC) ", 1080, 630);
}   
  