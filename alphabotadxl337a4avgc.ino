//NEKTARIOS KOURAKIS
#include "AlphaBot.h"
// Make sure these two variables are correct for your setup
int scale = 3; // 3 (±3g) for ADXL337, 200 (±200g) for ADXL377
 float ACCX;
 float ACCY;
//float ACCZ; 
 float SETAccX;
float SETAccY;
//float SETAccZ;
volatile float scaledX, scaledY;
//volatile float scaledZ;


void SET(){
int rawX = analogRead(A7);
int rawY = analogRead(A4);
 //int rawZ = analogRead(A6);
 
  
 scaledX = mapf(rawX, 0, 1023, -scale, scale); // 3.3/5 * 1023 =~ 675
 scaledY = mapf(rawY, 0, 1023, -scale, scale);
 //scaledZ = mapf(rawZ, 0, 1023, -scale, scale);
SETAccX=scaledX;
SETAccY=scaledY;
  
} 

//ΔΕΚΤΗΣ ΚΑΙ ΠΟΜΠΟΣ ΥΠΕΡΗΧΗΤΙΚΟΥ ΑΙΣΘΗΤΗΡΑ,ECHO AND TRIGGER ULTRASONIC SENSOR
int ECHO = 11;
int TRIG = 12;

int Distanceultra = 0;


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



// Constants for Interrupt Pins
// Change values if not using Arduino Uno
 
const byte MOTOR1 = 2;  // Motor 1 Interrupt Pin - INT 0
const byte MOTOR2 = 3;  // Motor 2 Interrupt Pin - INT 1
 
// Integers for pulse counters
unsigned int counter1 = 0;
unsigned int counter2 = 0;
 
// Float for number of slots in encoder disk
float diskslots = 20;  // Change to match value of encoder disk
 
// Interrupt Service Routines
 
// Motor 1 pulse count ISR
void ISR_count1()  
{
  counter1++;  // increment Motor 1 counter value
}
 
// Motor 2 pulse count ISR
void ISR_count2()  
{
  counter2++;  // increment Motor 2 counter value
}
 
// TimerOne ISR,ALL THIS FOR MEASURE SPEED OF TWO SIDES-TWO WHEELS
void ISR_timerone()
{
  Timer1.detachInterrupt();  // Stop the timer
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
  volatile int rawX = analogRead(A7);
 volatile int rawY = analogRead(A4);
 volatile int rawZ = analogRead(A6);

 scaledX = mapf(rawX, 0, 1023, -scale, scale); // 3.3/5 * 1023 =~ 675
  scaledY = mapf(rawY, 0, 1023, -scale, scale);
// scaledZ = mapf(rawZ, 0, 1023, -scale, scale);
  ACCX=(9.81*(scaledX-SETAccX)* 1.95); //1.95 διορθωτικος παραγοντας για να προκυπτουν σωστες τιμες επιταχυνσης
ACCY=(9.81*(scaledY-SETAccY)* 1.95);
//ACCZ=(9.81*(scaledZ)*1.98);
 volatile float ax[20]; // Πίνακας-αποθήκη 20 τελευταίων τιμών απόστασης
volatile float ay[20];
int ai;
volatile float sumx=0,sumy=0;
volatile float mox,moy;

volatile float accelx,accely;  
  for(ai=0; ai<10; ai++) // ολίσθησε αριστερά τις 19 τιμές στον πίνακα
    {
      ax[ai]=ax[ai+1];
      sumx=sumx+ax[ai]; // πρόσθεσε την τιμή στο άθροισμα τιμών
      ay[ai]=ay[ai+1];
      sumy=sumy+ay[ai]; // πρόσθεσε την τιμή στο άθροισμα τιμών
    }
  
    
  sumx=sumx+accelx; // τώρα το άθροισμα έχει όλες τις τιμές του πίνακα ενημερωμένου

  ax[19]=accelx; // αντικατάστησε την τελευταία θέση του a[] με την πρόσφατη τιμή

  mox=sumx/20; // μέσος όρος των 20 τιμών 
  
  
 
    
  sumy=sumy+accely; // τώρα το άθροισμα έχει όλες τις τιμές του πίνακα ενημερωμένου

  ay[19]=accely; // αντικατάστησε την τελευταία θέση του a[] με την πρόσφατη τιμή απόστασης 

  moy=sumy/20; // μέσος όρος των 20 τιμών απόστασης
  
   
  float speed1 =  rotation1 * 0.2076;
  float speed2 =  rotation2 * 0.2076;
  //Serial.print("motorA=");
  Serial.print(speed1);
  //Serial.println("  m/min");
 // Serial.print("motorB=");
Serial.print(",");///
 Serial.print(speed2);
 Serial.print(",");
 Serial.print(mox);
  Serial.print(",");
  Serial.println(moy);
 // Serial.print(",");
// Serial.print(ACCZ); Serial.println(" g");
 // Serial.println("  m/min");
 Timer1.attachInterrupt( ISR_timerone );  // Enable the timer 
}
 


void setup()
{
  
  Serial.begin(9600);
  SET();
  
  UltrasonicConfig();
  //FOR MEASURE SPEED
  Timer1.initialize(1000000); // set timer for 1sec
  attachInterrupt(digitalPinToInterrupt (MOTOR1), ISR_count1, RISING);  // Increase counter 1 when speed sensor pin goes High
  attachInterrupt(digitalPinToInterrupt (MOTOR2), ISR_count2, RISING);  // Increase counter 2 when speed sensor pin goes High
  Timer1.attachInterrupt( ISR_timerone ); // Enable the timer
  pinMode(IR, INPUT);
  ArduinoCar.SetSpeed(150);
  
 
  
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
      ArduinoCar.LeftCircle();
      break;

    case KEY2:
     // ArduinoCar.Forward();
     ArduinoCar.MotorRun(251,250);
      break;
      
//THIS CASE IS FOR centripetal acceleration ,THE OPEN CIRCLE WILL SHOW IN Y-AXIS THE centripetal acceleration
    case KEY3:
      ArduinoCar.MotorRun(180,230);
      delay(20000);
      break;
      
    case KEY4:
      ArduinoCar.Left();
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
    case KEY7:
    {Distance_test();
    //Measure the distance ahead 
    if (Distanceultra > 35)
    {
      Distance_test();
    start_time=millis();
     while ((millis() - start_time < 100000) && (Distanceultra > 35)) { // Εφόσον δεν πέρασαν 100 δευτ κάνε:
     Distance_test();   
      ArduinoCar.SetSpeed(100); 
      ArduinoCar.Forward(1600);
     // ArduinoCar.SetSpeed(65);  
      ArduinoCar.Left(550);
      Distance_test();  
      ArduinoCar.SetSpeed(120); 
      ArduinoCar.Forward(1500);
     // ArduinoCar.SetSpeed(65);  
      ArduinoCar.Left(550);
      Distance_test();  
      }
      }
      Distance_test();
      if (Distanceultra <= 35)  //*
      {
      resetFunc(); 
      delay(1000);
      }
      resetFunc();
      delay(1000);
      } 
      break;      

    case KEY8:
      ArduinoCar.Backward();
      break;

    case KEY9:
    {Distance_test();
    //Measure the distance ahead 
    if (Distanceultra > 35)
    {
      Distance_test();
    start_time=millis();
     while ((millis() - start_time < 20000) && (Distanceultra > 35)) { // Εφόσον δεν πέρασαν 20 δευτ κάνε:
     Distance_test();   
     for (int i=0; i <= 250; i++){
     ArduinoCar.MotorRun(i+1,i);  
   delay(100);
     }
    
    
     Distance_test(); 
     for (int i=0; i <= 250; i++){
     ArduinoCar.MotorRun(i+1,i);  
   delay(60);
     }
     Distance_test(); 
     ArduinoCar.MotorRun(254,250);  
     for (int i=250; i >= 0; i--){
     ArduinoCar.MotorRun(i+1,i);  
   delay(100);
     }
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
// Same functionality as Arduino's standard map function, except using floats
float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
