//NEKTARIOS KOURAKIS
#include "AlphaBot.h"

int sampleDelay = 20;   //number of milliseconds between readings

//ΔΕΚΤΗΣ ΚΑΙ ΠΟΜΠΟΣ ΥΠΕΡΗΧΗΤΙΚΟΥ ΑΙΣΘΗΤΗΡΑ,ECHO AND TRIGGER ULTRASONIC SENSOR
int ECHO = 11;
int TRIG = 12;

int Distanceultra = 0;

int deltaright;
int deltaleft;
void UltrasonicConfig()
{
  pinMode(ECHO, INPUT);                   // Define the ultrasonic echo input pin
  pinMode(TRIG, OUTPUT);                  // Define the ultrasonic trigger input pin
}

void Distance_test()                      // Measure the distance 
{
  digitalWrite(TRIG, LOW);                // set trig pin low 2μs
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);               // set trig pin 10μs , at last 10us 
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);                // set trig pin low
  float Fdistanceultra = pulseIn(ECHO, HIGH);  // Read echo pin high level time(us)
  
  Fdistanceultra = Fdistanceultra / 58;                
  //Y m =（X s * 344）/ 2; 
  //X s =（ 2 * Y m）/ 344;
  //X s = 0.0058 * Y m;
  //cm = us /58
  
 // Serial.print("Fdistance:");            //output distance (Unit: cm)
  //Serial.println(Fdistance);       
  Distanceultra = Fdistanceultra;
} 
//ΚΛΕΙΔΙΑ ΤΟΥ ΤΗΛΕΧΕΙΡΙΣΤΗΡΙΟΥ-ΟΡΙΣΜΟΣ,ΔΙΕΥΘΥΝΣΗ ΤΟΥΣ,KEY REMOTE IR CONTROL-ADRESS
#define KEY2 0x18                 //Key:2 
#define KEY8 0x52                 //Key:8 
#define KEY4 0x08                 //Key:4 
#define KEY6 0x5A                 //Key:6 
#define KEY1 0x0C                 //Key:1 
#define KEY3 0x5E                 //Key:3 
#define KEY5 0x1C                 //Key:5
#define KEY9 0x4A 
#define KEY7 0x42 
#define SpeedDown 0x07            //Key:VOL-
#define SpeedUp 0x15              //Key:VOL+
#define ResetSpeed 0x09           //Key:EQ
#define Repeat 0xFF               //press and hold the key

#define IR  4                     //he infrare remote receiver pin 
unsigned long n = 0;
AlphaBot ArduinoCar = AlphaBot();
// Include the TimerOne Library from Paul Stoffregen
#include "TimerOne.h"

int velocity=20;

unsigned char t[30];
unsigned char results;
char IR_decode(unsigned char code);
void translateIR();
unsigned long start_time; // Δημιούργησε μια μεταβλητή τύπου unsigned long integer με όνομα start_time

#include "TRSensors.h"

                                                                                                                                                                                                                                                        
int Lpwm_val=150;
int  Rpwm_val=150;
// Constants for Interrupt Pins
// Change values if not using Arduino Uno
 
const byte MOTOR1 = 2;  // Motor 1 Interrupt Pin - INT 0
const byte MOTOR2 = 3;  // Motor 2 Interrupt Pin - INT 1
 
// Integers for pulse counters
unsigned int counter1,circles1 = 0;
unsigned int counter2,circles2 = 0;
 
// Float for number of slots in encoder disk
float diskslots = 20;  // Change to match value of encoder disk
float Sdistance=0; 
volatile float horizontalangle=0;
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
 horizontalangle = (angle_right1 - angle_left1)/100;
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
  
  float speed1 =  rotation1 * 0.208;
  float speed2 =  rotation2 * 0.208;
   Sdistance=(((circles1+circles2)/2) / diskslots)*0.208; 
  //Serial.print("motorA=");
  Serial.print(speed1);
  //Serial.println("  m/min");
 // Serial.print("motorB=");
Serial.print(",");///
 Serial.print(speed2);
 Serial.print(",");
 Serial.print(Sdistance);  
  Serial.print(",");

  Serial.println(horizontalangle);
  
  
 Timer1.attachInterrupt( ISR_timerone );  // Enable the timer 
}
 


void setup()
{
  
  Serial.begin(9600);
 
 
  UltrasonicConfig();
  //FOR MEASURE SPEED
  Timer1.initialize(1000000); // set timer for 1sec
  attachInterrupt(digitalPinToInterrupt (MOTOR1), ISR_count1, RISING);  // Increase counter 1 when speed sensor pin goes High
  attachInterrupt(digitalPinToInterrupt (MOTOR2), ISR_count2, RISING);  // Increase counter 2 when speed sensor pin goes High
  Timer1.attachInterrupt( ISR_timerone ); // Enable the timer
  pinMode(IR, INPUT);
  
  ArduinoCar.SetSpeed(Lpwm_val,Rpwm_val);
  
 
  
  delay(1000);
  
}

void(*resetFunc)(void)=0;
//THE MAIN BONE OF THIS PROGRAM IS IR REMOTE CONTROL WITH ADDITIONAL FUNCTIONALITIES
void loop()
{
  if (IR_decode(&results))
  {
    translateIR();
  }

    if (millis() - n > 200)
  {
    ArduinoCar.Brake();
  }
}

/*-----( Declare User-written Functions )-----*/
void translateIR() // takes action based on IR code received
// describing KEYES Remote IR codes
{
  n = millis(); ;
  switch (results)
  {
    case KEY1:
      ArduinoCar.RightCircle();
      break;

    case KEY2:
     // ArduinoCar.Forward();
     ArduinoCar.MotorRun(Lpwm_val,Rpwm_val);
     delay(10000);
      break;
      
//THIS CASE IS FOR centripetal acceleration ,THE OPEN CIRCLE WILL SHOW IN Y-AXIS THE centripetal acceleration
    case KEY3:
      ArduinoCar.MotorRun(110,180);
      delay(20000);
      break;
      
    case KEY4:
      ArduinoCar.Left();
      break;

     case SpeedDown:
     Distance_test(); 
    
    ArduinoCar.MotorRun(255,255);  
     for (int i=0; i <= 225; i++){
     ArduinoCar.MotorRun(255-i,255-i);  
   delay(150);
     }
     
     break;
      
//OBSTACLE AVOID CASE
    case KEY5:
      start_time=millis();
while (millis() - start_time < 100000) {
       Distance_test();                      //Measure the distance ahead
    
    if((Distanceultra < 48) && (Distanceultra > 25))                     //The value is the distance to the obstacle and can be set according to the actual situation
     // while((Distanceultra < 48) && (Distanceultra > 25))
      {
        ArduinoCar.Right();                     //turn right
        Distance_test();
        
      }
    else{
      Distance_test();
      if(Distanceultra >= 48)
     // while (Distanceultra >= 48)
      {
      ArduinoCar.Forward();
       //there is no objtacle ,go straight 
       Distance_test(); 
      }
      if(Distanceultra <= 25)
     // while (Distanceultra <=25)
      {
       ArduinoCar.Backward();
       Distance_test();  
      }
      }
      Distance_test();
      }
      break;

    case KEY6:
      ArduinoCar.Right();
      break;
      
//VISUAL TRACK WITH ESCAPE* FUNCTION
    case KEY9:
    deltaright=0;
    if(Rpwm_val+1<=254&&Rpwm_val+1<=254)
    {
              Rpwm_val+=1;
              deltaright+=1;
        ArduinoCar.SetSpeed(Lpwm_val,Rpwm_val);                                                   //Set_Speed(Lpwm_val,Rpwm_val);
         Serial.print(" +1 velocity right: "); Serial.print(Lpwm_val);  Serial.println(Rpwm_val);
    }
      break;      

     case KEY7:
     deltaleft=0;
    if(Rpwm_val+1<=254&&Rpwm_val+1<=254)
      {
              Lpwm_val+=1;
              deltaleft+=1;
        ArduinoCar.SetSpeed(Lpwm_val,Rpwm_val);                                                   //Set_Speed(Lpwm_val,Rpwm_val);
         Serial.print(" +1 velocity left: "); Serial.print(Lpwm_val);  Serial.println(Rpwm_val); 
      }
      break;      
      
   /* case KEY8:
      ArduinoCar.Backward();
      break;*/

    case KEY8:
    {Distance_test();
    //Measure the distance ahead 
    if (Distanceultra > 35)
    {
      Distance_test();
    start_time=millis();
     while ((millis() - start_time < 20000UL) && (Distanceultra > 35)) { // Εφόσον δεν πέρασαν 20 δευτ κάνε:
     Distance_test();   
     for (int i=40; i <= 170; i++){
     ArduinoCar.MotorRun(i+deltaright,i+deltaleft);  
   delay(150);
     }
       for (int i=170; i <= 241; i++){
     ArduinoCar.MotorRun(i,i);  
   delay(150);
     }
   
    
  /*  Distance_test(); 
     for (int i=0; i <= 240; i++){
     ArduinoCar.MotorRun(i+deltaright,i+deltaleft);  
   delay(60);
     }*/
    
     }
     Distance_test();  
      }
      }
      Distance_test();
      if (Distanceultra <= 35)  //*
      {
      resetFunc(); 
      delay(1000);
      }
      //resetFunc();
     // delay(1000);
       
     
     
  break;

    case Repeat:
      break;

    default:
      ArduinoCar.Brake();
  }// End Case
} //END translateIR

char IR_decode(unsigned char * code)
{
  char flag = 0;
  unsigned int count = 0;
  unsigned char i, index, cnt = 0, data[4] = {0, 0, 0, 0};
  if (digitalRead(IR) == LOW)
  {
    count = 0;
    while (digitalRead(IR) == LOW && count++ < 200)  //9ms
      delayMicroseconds(60);
    t[0] = count;
    count = 0;
    while (digitalRead(IR) == HIGH && count++ < 80)   //4.5ms
      delayMicroseconds(60);
    t[1] = count;
    for (i = 0; i < 32; i++)
    {
      count = 0;
      while (digitalRead(IR) == LOW && count++ < 15) //0.56ms
        delayMicroseconds(60);
      count = 0;
      while (digitalRead(IR) == HIGH && count++ < 40) //0: 0.56ms; 1: 1.69ms
        delayMicroseconds(60);
      if (count > 20)data[index] |= (1 << cnt);
      if (cnt == 7)
      {
        cnt = 0;
        index++;
      }
      else cnt++;
    }
    if (data[0] + data[1] == 0xFF && data[2] + data[3] == 0xFF) //check
    {
      code[0] = data[2];
      //Serial.println(code[0], HEX);
      flag = 1;
    }
    if (data[0] == 0xFF && data[1] == 0xFF && data[2] == 0xFF && data[3] == 0xFF)
    {
      code[0] = 0xFF;
      //Serial.println("rep");
      flag = 1;
    }
  }
  return flag;
}
