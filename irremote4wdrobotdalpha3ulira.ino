#include <LiquidCrystal_I2C.h> 
#include <IRremote.h>
// Include IR Remote Library by Ken Shirriff
unsigned long start_time; // Δημιούργησε μια μεταβλητή τύπου unsigned long integer με όνομα start_time
// Define sensor pin
const int RECV_PIN = 4;
// Define integer to remember toggle state
int togglestate = 0;


//ultrasonicservo
#include "AlphaBot.h"

int ECHO = 12;
int TRIG = 11;

int FrontDistance = 0;
int LeftDistance = 0;
int RightDistance = 0;

int ServoPin = 9;                            //Set the SG90 servo drive pin to digital port 9  ()
int myangle;                                 //Defines the angle variable
int pulsewidth;                              //Defines the pulse width variable
int val;

AlphaBot Car1 = AlphaBot();

void UltrasonicConfig()
{
  pinMode(ECHO, INPUT);                      // Define the ultrasonic echo input pin
  pinMode(TRIG, OUTPUT);                     // Define the ultrasonic trigger input pin
}

void ServoConfig()
{
  pinMode(ServoPin,OUTPUT);                  //Set the servo output interface
}

float DistanceTest()                         // Measure the distance  
{
  digitalWrite(TRIG, LOW);                   // set trig pin low 2μs
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);                  // set trig pin 10μs , at last 10us 
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);                   // set trig pin low
  float Fdistance = pulseIn(ECHO, HIGH);     // Read echo pin high level time(us)
  Fdistance= Fdistance / 58;        
  //Y m=（X s*344）/2
  //X s=（ 2*Y m）/344 
  //X s=0.0058*Y m 
  //cm = us /58
  
  //Serial.print("Fdistance:");               //output distance (Unit: cm)
 // Serial.println(Fdistance);
  return Fdistance;
}  

void DistanceDisplay(int Distance)          //display distance
{
  if((2<Distance)&(Distance<400))           //Ultrasonic range ranging 2cm to 400cm
  {
   // Serial.print("Distance:");             //print distance
   // Serial.print(Distance);      
  //  Serial.print("cm\n\n");        
  }
  else
  {
    //Serial.print("!!! Out of range\n");      
  }
  delay(250);
}

void ServoPulse(int ServoPin,int myangle)    /*Defines an impulse function that is used to simulate PWM generation*/
{
  pulsewidth=(myangle*11)+500;              //The angle is converted to a pulse width value of 500-2480
  digitalWrite(ServoPin,HIGH);              //Set the servo Pin level high
  delayMicroseconds(pulsewidth);            //delay the pulse width value
  digitalWrite(ServoPin,LOW);               //Set the servo Pin level low
  delay(20-pulsewidth/1000);                //delay the remaining time of period
}

void FrontDetection()
{
  //Here the number of cycles decreased, in order to increase the speed of the car encounter obstacles
  for(int i=0;i<=5;i++)                     //The number of PWM,the equivalent delay to ensure that can go to the response angle
  {
    ServoPulse(ServoPin,90);                //analog out PWM
  }
  FrontDistance = DistanceTest();
  //Serial.print("FrontDistance:");     
  // Serial.println(FrontDistance);        
  //DistanceDisplay(FrontDistance);
}

void LeftDetection()
{
  for(int i=0;i<=15;i++)                   //The number of PWM,the equivalent delay to ensure that can go to the response angle
  {
    ServoPulse(ServoPin,175);              //analog out PWM
  }
  LeftDistance = DistanceTest();
  //Serial.print("LeftDistance:");      
  //Serial.println(LeftDistance);         
}

void RightDetection()
{
  for(int i=0;i<=15;i++) 
  {
    ServoPulse(ServoPin,35);
  }
  RightDistance = DistanceTest();
  //Serial.print("RightDistance:");      
  //Serial.println(RightDistance);         
}

  
 //      0xFFA25D, // CH-
           /* 0xFF629D,   // CH  avoid
            0xFFE21D,  // CH+

            0xFF22DD, // |<<
            0xFF02FD, // >>|
            0xFFC23D, // >||          

            0xFFE01F, // -
            0xFFA857, // +
            0xFF906F, // EQ  big circle

            0xFF6897, // 0  medium cyrcle
            0xFF9867, // 100+
            0xFFB04F, // 200+

            0xFF30CF, // 1  +5 velocity right    
            0xFF18E7, // 2 forward
            0xFF7A85, // 3  -5 velocity both         

            0xFF10EF, // 4 left
            0xFF38C7, // 5  stop
            0xFF5AA5,  // 6 right

            0xFF42BD, // 7  +1 velocity left
            0xFF4AB5, // 8  backward
            0xFF52AD  // 9  +1 velocity right                             */  



   
// Define IR Receiver and Results Objects
IRrecv irrecv(RECV_PIN);
decode_results results;
 //INTERRUPTS
volatile float horizonangle = 0;
#include "TimerOne.h"
const byte MOTOR1 = 2;  // Motor 1 Interrupt Pin - INT 0
const byte MOTOR2 = 3;  // Motor 2 Interrupt Pin - INT 1
 
// Integers for pulse counters
unsigned int counter1,circles1 = 0;
unsigned int counter2,circles2 = 0;
 
// Float for number of slots in encoder disk
float diskslots = 40;  // Change to match value of encoder disk
float Sdistance=0; 
// Interrupt Service Routines
 
// Motor 1 pulse count ISR
void ISR_count1()  
{
  counter1++;  // increment Motor 1 counter value
  circles1++;
}
 
// Motor 2 pulse count ISR
void ISR_count2()  
{
  counter2++;  // increment Motor 2 counter value
  circles2++;
}
// TimerOne ISR,ALL THIS FOR MEASURE SPEED OF TWO SIDES-TWO WHEELS
void ISR_timerone()
{
  Timer1.detachInterrupt();  // Stop the timer
   
   int angle_left1 = (circles1 % 360) * (90/80) ;
 int angle_right1 = (circles2 % 360) * (90/80) ;
 horizonangle = (angle_right1 - angle_left1)/100;
  //Serial.print("Motor Speed 1: ");
  float rotation1 = (counter1 / diskslots) * 60.00;  // calculate RPM for Motor 1
  //Serial.print(rotation1);  
  //Serial.print(" RPM - ");
  counter1 = 0;  //  reset counter to zero
  //Serial.print("Motor Speed 2: ");
  float rotation2 = (counter2 / diskslots) * 60.00;  // calculate RPM for Motor 2
  //Serial.print(rotation2);  
  //erial.println(" RPM");
  counter2 = 0;  //  reset counter to zero
 Sdistance=(((circles1+circles2)/2) / diskslots)*0.208;   //0.208=π*δ , δ=diameter=2*radius
  float speed1 = 2* rotation1 * 0.208; //*2 because of 0,5secinterrupt
 float speed2 = 2* rotation2 * 0.208;  //radius=0.0333m
  //Serial.print("motorA=");
 Serial.print(speed1);
  //Serial.println("  m/min");
 // Serial.print("motorB=");
 Serial.print(",");
 Serial.print(speed2);
 Serial.print(",");
  Serial.print(Sdistance);
   Serial.print(",");
  
  Serial.println(horizonangle);
  // Serial.println("  m/min");
  
 Timer1.attachInterrupt( ISR_timerone );  // Enable the timer 
}
 
LiquidCrystal_I2C lcd(0x27,16,2); //定义一个液晶
#define Lpwm_pin 6 //在這調速   //9
#define Rpwm_pin 5 //在這調速 //
int pinLB=A2; // 定義2腳位 左後
int pinLF=A3; // 定義4腳位 左前
int pinRB=A1; // 定義7腳位 右後
int pinRF=A0; // 定義8腳位 右前  //6
unsigned char Lpwm_val = 160;
unsigned char Rpwm_val = 175;
int Car_state=0;
void M_Control_IO_config(void)
{
pinMode(pinLB,OUTPUT); 
pinMode(pinLF,OUTPUT); 
pinMode(pinRB,OUTPUT); 
pinMode(pinRF,OUTPUT); 
pinMode(Lpwm_pin,OUTPUT); // (PWM)
pinMode(Rpwm_pin,OUTPUT); //  (PWM)
}
void Set_Speed(unsigned char Left,unsigned char Right)
{
analogWrite(Lpwm_pin,Left);
analogWrite(Rpwm_pin,Right);
}
void advance(void) // 前進
{

digitalWrite(pinLB,LOW); // 使馬達（左後）動作
digitalWrite(pinLF,HIGH);
digitalWrite(pinRB,LOW); // 使馬達（右後）動作
digitalWrite(pinRF,HIGH);
Car_state = 1;

}
void turnR(void) //右轉(雙輪)
{
digitalWrite(pinRB,LOW); //使馬達（右後）動作
digitalWrite(pinRF,HIGH);
digitalWrite(pinLB,HIGH);
digitalWrite(pinLF,HIGH); //使馬達（左前）動作
Car_state = 4;

}
void turnL(void) //左轉(雙輪)
{
digitalWrite(pinRB,HIGH);
digitalWrite(pinRF,HIGH); //使馬達（右前）動作
digitalWrite(pinLB,LOW); //使馬達（左後）動作
digitalWrite(pinLF,HIGH);
Car_state = 3;

}
void stopp(void) //停止
{
digitalWrite(pinRB,HIGH);
digitalWrite(pinRF,HIGH);
digitalWrite(pinLB,HIGH);
digitalWrite(pinLF,HIGH);
Car_state = 5;

}
void back(void) //後退
{

digitalWrite(pinLB,HIGH); //使馬達（左後）動作
digitalWrite(pinLF,LOW);
digitalWrite(pinRB,HIGH); //使馬達（右後）動作
digitalWrite(pinRF,LOW);
Car_state = 2;

}

void LCD1602_init(void) //液晶初始化用到的函数，包罗在这里
{
lcd.init(); //调用LiquidCrystal_I2C.h里的LCD初始化函数
delay(10); //延时10毫秒
lcd.backlight(); //开LCD1602的背光灯
lcd.clear(); //清除屏幕
}

void setup()
{ 
  //INTERRUPTS
  Serial.begin(9600);
  
 
  //FOR MEASURE SPEED
  Timer1.initialize(500000); // set timer for 1sec
  attachInterrupt(digitalPinToInterrupt (MOTOR1), ISR_count1, CHANGE);  // Increase counter 1 when speed sensor pin goes High
  attachInterrupt(digitalPinToInterrupt (MOTOR2), ISR_count2, CHANGE);  // Increase counter 2 when speed sensor pin goes High
  Timer1.attachInterrupt( ISR_timerone ); // Enable the timer
  
  // Enable the IR Receiver
  irrecv.enableIRIn();
LCD1602_init();
M_Control_IO_config();
Set_Speed(Lpwm_val,Rpwm_val);

lcd.setCursor(0, 0); //光标设置在第一行，第一列，也就是左上角
lcd.print(" Wait Signal ");
stopp();
 UltrasonicConfig();
  ServoConfig();
}

void(*resetFunc)(void)=0;

void loop()
{ 
 FrontDetection();
       while(FrontDistance > 7) 
{
  
 if (irrecv.decode(&results)){
 
        switch(results.value){

        
          
          case 0xFF18E7:   //2
        advance();
       // Serial.print(" GO  "); Serial.print(Lpwm_val);  Serial.println(Rpwm_val);
        lcd.setCursor(0, 1);
        lcd.print("  Go   ");   //Serial.print("\n GO");
        lcd.print(Lpwm_val);  lcd.print(Rpwm_val);
        delay(3000);
        break;
   
          case 0xFF4AB5: 
        back();
       // Serial.print(" back  "); Serial.print(Lpwm_val);  Serial.println(Rpwm_val);
        lcd.setCursor(0, 1);
        lcd.print("Back   ");   //Serial.print("\n Back");
        lcd.print(Lpwm_val);  lcd.print(Rpwm_val);
        delay(2000);
        break;

        case 0xFF10EF:
        turnL();
       // Serial.print(" turn left  "); Serial.print(Lpwm_val);  Serial.println(Rpwm_val);
        lcd.setCursor(0, 1);
        lcd.print("Left   ");  //Serial.print("\n Left");
        lcd.print(Lpwm_val);  lcd.print(Rpwm_val);
        delay(3000);
        break;

        case 0xFF5AA5:   
        turnR();
       // Serial.print(" turn right  "); Serial.print(Lpwm_val);  Serial.println(Rpwm_val);
        lcd.setCursor(0, 1);
        lcd.print("Right   ");  //Serial.print("\n Right");
        lcd.print(Lpwm_val);  lcd.print(Rpwm_val);
        delay(3000);
        break;

        case 0xFF38C7:
        stopp();
       // Serial.print(" stop  "); Serial.print(Lpwm_val);  Serial.println(Rpwm_val);
        lcd.setCursor(0, 1);
        lcd.print("Stop      ");  //Serial.print("\n Stop");
        lcd.print(Lpwm_val);  lcd.print(Rpwm_val);
        delay(3000);
        break;

        case  0xFF6897:   //0
        Set_Speed(255,80);
        advance();
       // Serial.print(" medium circle  "); Serial.print(Lpwm_val);  Serial.println(Rpwm_val);
        lcd.setCursor(0, 1);
        lcd.print("big circle  ");   //Serial.print("\n GO");
        lcd.print(Lpwm_val);  lcd.print(Rpwm_val);
        delay(3000);
        break;

        case  0xFF906F:   //EQ
        Set_Speed(255,130);
        advance();
       // Serial.print(" big circle  "); Serial.print(Lpwm_val);  Serial.println(Rpwm_val);
        lcd.setCursor(0, 1);
        lcd.print("big circle  ");   //Serial.print("\n GO");
        lcd.print(Lpwm_val);  lcd.print(Rpwm_val);
        delay(3000);
        break;

        case 0xFF30CF:     //3
        if(Rpwm_val+5<=255&&Rpwm_val+5<=255){
               Rpwm_val+=5;
              Set_Speed(Lpwm_val,Rpwm_val);
         Serial.print(" +5 velocity right:  "); Serial.print(Lpwm_val);  Serial.println(Rpwm_val);     
              lcd.setCursor(0, 1);
        lcd.print("+5vright "); lcd.print(Lpwm_val);  lcd.print(Rpwm_val);
        }
        break;
        
        case 0xFF7A85:   //1
        if(Rpwm_val>=0&&Rpwm_val>=0){
        Lpwm_val-=5; Rpwm_val-=5;
        Set_Speed(Lpwm_val,Rpwm_val);
        Serial.print(" -5 velocity : "); Serial.print(Lpwm_val);  Serial.println(Rpwm_val); 
        lcd.setCursor(0, 1);
        lcd.print("-velocity "); lcd.print(Lpwm_val);  lcd.print(Rpwm_val);
        }
        break;

         case 0xFF42BD:   //7
        if(Rpwm_val+1<=255&&Rpwm_val+1<=255){
              Lpwm_val+=1; 
              Set_Speed(Lpwm_val,Rpwm_val);
         Serial.print(" +1 velocity left: "); Serial.print(Lpwm_val);  Serial.println(Rpwm_val);     
              lcd.setCursor(0, 1);
        lcd.print("+left    "); lcd.print(Lpwm_val); lcd.print(Rpwm_val);
        }
        break;

         case 0xFF52AD:   //9
        if(Rpwm_val+1<=255&&Rpwm_val+1<=255){
               Rpwm_val+=1;
              Set_Speed(Lpwm_val,Rpwm_val);
         Serial.print(" +1 velocity right:  "); Serial.print(Lpwm_val);  Serial.println(Rpwm_val);      
              lcd.setCursor(0, 1);
        lcd.print("+right   "); lcd.print(Lpwm_val); lcd.print(Rpwm_val);
        }
        break;

        case 0xFF629D:             //ch ultra
       // advance();
       // Serial.print(" GO  "); Serial.print(Lpwm_val);  Serial.println(Rpwm_val);
     
       FrontDetection();
       while(FrontDistance > 9)
       {
      FrontDetection();   
    if(FrontDistance < 32)                              //there is an obstacle 
    {
      back();
      delay(220);
      
      stopp();                                     //stop 
      delay(200);
      
      LeftDetection();                                  //Measure the distance to the left obstacle
      DistanceDisplay(LeftDistance);                    //display distance
      RightDetection();                                 //Measure the distance to the right obstacle
      DistanceDisplay(RightDistance);                   //display distance
      if((LeftDistance < 35 ) &&( RightDistance < 35 )) //When the left and right sides are relatively close by the obstacles
      {
        turnR();                              //Turn around
        delay(60);
      }
      else if(LeftDistance > RightDistance)             //The left is far more than the right
      {      
        turnL();                                    //Turn left
        delay(300);
        stopp();                                   //stop
        delay(100);
      }
      else                                              //The right is far more than the left
      {
        turnR();                                   //turn right
        delay(300);
        stopp();                                   //stop
        delay(100);
      }
    }
    else
    {
      advance();                                   //there is no objtacle ,go straight    advance();
    }
       }
     stopp();   
        break;





        
        
         case 0xFFA857 : //+
        
        
        for(int i=0; i<244;i++ ){
               
               Set_Speed(i,i+12);
              advance();
              lcd.setCursor(0, 1);
        lcd.print("+accel   "); lcd.print(Lpwm_val); lcd.print(Rpwm_val);
        delay(45);
        }
        
       
        break;

        case 0xFFE01F :  //-
        
        
        for(int j=250; j>=30; j-- ){
               
              Set_Speed(j,j+4);
              advance();
              lcd.setCursor(0, 1);
        lcd.print("-accel   "); lcd.print(Lpwm_val); lcd.print(Rpwm_val);
        delay(25);
        if (j < 35)  
      {
      resetFunc(); 
      delay(100);
      }
        }
        stopp();
        break;

        default:
        stopp();
        break;  
        }
        
      
       
       
        
        
   FrontDetection(); 
   irrecv.resume(); 
  }
}
stopp();
FrontDetection();
}
