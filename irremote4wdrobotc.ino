#include <LiquidCrystal_I2C.h> 
#include <IRremote.h>
// Include IR Remote Library by Ken Shirriff

// Define sensor pin
const int RECV_PIN = 12;
// Define integer to remember toggle state
int togglestate = 0;
 
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
int pinLB=8; // 定義2腳位 左後
int pinLF=7; // 定義4腳位 左前
int pinRB=4; // 定義7腳位 右後
int pinRF=9; // 定義8腳位 右前  //6
unsigned char Lpwm_val = 180;
unsigned char Rpwm_val = 180;
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
}
void loop()
{
 if (irrecv.decode(&results)){
 
        switch(results.value){
          case 0xFF18E7:
        advance();
        lcd.setCursor(0, 1);
        lcd.print(" Go ");   //Serial.print("\n GO");
        delay(3000);
        break;
   
          case 0xFF4AB5: 
        back();
        lcd.setCursor(0, 1);
        lcd.print("Back ");   //Serial.print("\n Back");
        delay(2000);
        break;

        case 0xFF10EF:
        turnL();
        lcd.setCursor(0, 1);
        lcd.print("Left ");  //Serial.print("\n Left");
        delay(3000);
        break;

        case 0xFF5AA5:
        turnR();
        lcd.setCursor(0, 1);
        lcd.print("Right");  //Serial.print("\n Right");
        delay(3000);
        break;

        case 0xFF38C7:
        stopp();
        lcd.setCursor(0, 1);
        lcd.print("Stop ");  //Serial.print("\n Stop");
        delay(3000);
        break;


        case 0xFF30CF:
        if(Rpwm_val+10<=250&&Rpwm_val+10<=250){
              Lpwm_val+=10; Rpwm_val+=10;
              Set_Speed(Lpwm_val,Rpwm_val);
              
              lcd.setCursor(0, 1);
        lcd.print("+accel "); lcd.print(Rpwm_val);
        }
        break;
        
        case 0xFF7A85:
        if(Rpwm_val-10>=0&&Rpwm_val-10>=0){
        Lpwm_val-=10; Rpwm_val-=10;
        Set_Speed(Lpwm_val,Rpwm_val);
        
        lcd.setCursor(0, 1);
        lcd.print("-accel "); lcd.print(Lpwm_val); 
        }
        break;

         case 0xFF42BD:
        if(Rpwm_val+10<=250&&Rpwm_val+10<=250){
              Lpwm_val+=1; 
              Set_Speed(Lpwm_val,Rpwm_val);
              
              lcd.setCursor(0, 1);
        lcd.print("+accel "); lcd.print(Lpwm_val); lcd.print(Rpwm_val);
        }
        break;

         case 0xFF52AD:
        if(Rpwm_val+10<=250&&Rpwm_val+10<=250){
               Rpwm_val+=1;
              Set_Speed(Lpwm_val,Rpwm_val);
              
              lcd.setCursor(0, 1);
        lcd.print("+accel "); lcd.print(Lpwm_val); lcd.print(Rpwm_val);
        }
        break;
        }
        
        
    
    irrecv.resume(); 
  }
}
