// these constants describe the pins. They won't change:
const int xpin = A0;                  // x-axis of the accelerometer
const int ypin = A1;                  // y-axis
const int zpin = A2;                  // z-axis (only on 3-axis models)




int sampleDelay = 20;   //number of milliseconds between readings

void setup()
{
  // initialize the serial communications:
  Serial.begin(9600);
  analogReference(EXTERNAL);

  pinMode(xpin, INPUT);
  pinMode(ypin, INPUT);
  pinMode(zpin, INPUT);

}  




void loop()
{
  int x = analogRead(xpin);
  delay(1); 

  int y = analogRead(ypin);
  delay(1); 

  int z = analogRead(zpin);

  float zero_G_x = 508;
  float zero_G_y = 498;
  float zero_G_z = 511;

  float scale_x = 101;
  float scale_y = 102;
  float scale_z = 102;

  Serial.print(((float)x - zero_G_x)/scale_x);
  Serial.print("\t");

  Serial.print(((float)y - zero_G_y)/scale_y);
  Serial.print("\t");

  Serial.print(((float)z - zero_G_z)/scale_z);
  Serial.print("\n");  // delay before next reading:
  delay(sampleDelay);
}
