#include <SparkFun_MAG3110.h>
int E1 = 5;  
int M1 = 4; // Right
int E2 = 6;                      
int M2 = 7;// Left  
String comdata = "";
String c1 = "";
int c2=0;
float TurnRate = 0;
int HeadValue = 0;
MAG3110 mag = MAG3110(); //Instantiate MAG3110

void setup() {
  Serial.begin(9600);

  mag.initialize(); //Initialize the MAG3110
}

void loop() {

  float error=2;
 
////////////////////////////////////////////
while (Serial.available() > 0)  
    {
        comdata = "";
        comdata =Serial.readString();
        int FirstSpace = comdata.indexOf(" ");
        c1 = comdata.substring(0,FirstSpace);
        int SecondSpace = comdata.indexOf(" ");
        c2 = comdata.substring(FirstSpace,comdata.length()).toInt();
        Serial.println(c1);
        Serial.println(c2);
        if (c1=="a")
        {
          TurnRate = HeadValue + c2;
          Serial.print("TurnRate:");
          Serial.println(TurnRate);
          }
        if (c1=="d")
        {
          TurnRate = HeadValue - c2;
          Serial.print("TurnRate:");
          Serial.println(TurnRate);
          }
    }  
///////////////////////////////////////////

  if(!mag.isCalibrated()) //If we're not calibrated
  {
    if(!mag.isCalibrating()) //And we're not currently calibrating
    {
      Serial.println("Entering calibration mode");
      mag.enterCalMode(); //This sets the output data rate to the highest possible and puts the mag sensor in active mode
    }
    else
    {
      //Must call every loop while calibrating to collect calibration data
      //This will automatically exit calibration
      //You can terminate calibration early by calling mag.exitCalMode();
      mag.calibrate(); 
    }
  }
  else
  {
    Serial.println("Calibrated!");
  }
////////////////////////////////////////////////////////
  Serial.print("TurnRate:");
  Serial.println(TurnRate);
  HeadValue = mag.readHeading();
  Serial.print("Present direction:");
  Serial.print(HeadValue);
  Serial.print(";");
  Serial.print(" Ideal direction:");
  Serial.println(TurnRate);
if (HeadValue>TurnRate+error)
{
  TurnRight(120);
  }
if (HeadValue<TurnRate-error)
{
  TurnLeft(120);
  }
if (HeadValue>=TurnRate-error&&HeadValue<=TurnRate+error)
{
   analogWrite(E1, 0); 
   analogWrite(E2, 0);
  }
 delay(50);
}

void TurnLeft(int sped)
{
   digitalWrite(M1,LOW);//moving left
   digitalWrite(M2,HIGH);
   analogWrite(E1, sped); 
   analogWrite(E2, sped);
  }
void TurnRight(int sped)
{  
   digitalWrite(M1,HIGH);//moving right
   digitalWrite(M2,LOW);
   analogWrite(E1, sped); 
   analogWrite(E2, sped);
   
  }
void MoveForward(int sped)
{
   digitalWrite(M1,LOW);// moving forward
   digitalWrite(M2,LOW);
   analogWrite(E1,sped);
   analogWrite(E2,sped);
  }
void MoveBackward(int sped)
{
   digitalWrite(M1,HIGH);// moving backward
   digitalWrite(M2,HIGH);
   analogWrite(E1,sped);
   analogWrite(E2,sped);  
  }
 
