//NEKTARIOS KOURALKIS
#include "AlphaBot.h"
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
//line follow
#include "TRSensors.h"

#define NUM_SENSORS 5
// sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
TRSensors trs =   TRSensors();
unsigned int sensorValues[NUM_SENSORS];
unsigned int last_proportional = 0;
long integral = 0;

int ENA = 5;
int ENB = 6;
int IN1 = A1;
int IN2 = A0;
int IN3 = A2;
int IN4 = A3; 
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
  Timer1.attachInterrupt( ISR_timerone );  // Enable the timer
  float speed1 =  rotation1 * 0.2076;
  float speed2 =  rotation2 * 0.2076;
  //Serial.print("motorA=");
  Serial.print(speed1);
  //Serial.println("  m/min");
 // Serial.print("motorB=");
Serial.print(",");///
 Serial.println(speed2);
 // Serial.println("  m/min");
  
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
  ArduinoCar.SetSpeed(150);
  //THIS FOR LINE FOLLOWING
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA,0);
  analogWrite(ENB,0);
  for (int i = 0; i < 400; i++)                  // make the calibration take about 10 seconds
  {
    trs.calibrate();                             // reads all sensors 10 times
  }
  //Serial.println("calibrate done");
  
  // print the calibration minimum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++)
  {
   // Serial.print(trs.calibratedMin[i]);
   // Serial.print(' ');
  }
  //Serial.println();
  
  // print the calibration maximum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++)
  {
   // Serial.print(trs.calibratedMax[i]);
    //Serial.print(' ');
  }
  //Serial.println();
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
      ArduinoCar.Forward();
      break;
      
//THIS CASE IS FOR centripetal acceleration ,THE OPEN CIRCLE WILL SHOW IN Y-AXIS THE centripetal acceleration
    case KEY3:
      ArduinoCar.MotorRun(137,200);
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
    
    start_time=millis();
while (millis() - start_time < 200000)
{ 
  
  // Εφόσον δεν πέρασαν 200 δευτ κάνε:ακολουθησε την μαυρη γραμμη
      // Get the position of the line.  Note that we *must* provide
  // the "sensors" argument to read_line() here, even though we
  // are not interested in the individual sensor readings.
  unsigned int position = trs.readLine(sensorValues);
  //  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  //  {
  //    Serial.print(sensorValues[i]);
  //    Serial.print('\t');
  //  }
  //Serial.println(position); // comment this line out if you are using raw values

  // The "proportional" term should be 0 when we are on the line.
  int proportional = (int)position - 2000;
  // improve performance.
  int power_difference = proportional/15; //+derivative;  

  // Compute the actual motor settings.  We never set either motor
  // to a negative value.
  const int maximum =100;

  if (power_difference > maximum)
    power_difference = maximum;
  if (power_difference < - maximum)
    power_difference = - maximum;
//    Serial.println(power_difference);
  if (power_difference < 0)
  {
    analogWrite(ENB,maximum + power_difference);
    analogWrite(ENA,maximum);
  }
  else
  {
    analogWrite(ENB,maximum);
    analogWrite(ENA,maximum - power_difference);
  }    
      }
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
