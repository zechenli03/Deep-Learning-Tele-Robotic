#include <SparkFun_MAG3110.h>
int E1 = 5;  
int M1 = 4; // Right
int E2 = 6;                      
int M2 = 7;// Left  
MAG3110 mag = MAG3110(); //Instantiate MAG3110

void setup() {
  Serial.begin(9600);

  mag.initialize(); //Initialize the MAG3110
}

void loop() {

  float indirect=0;
  float error=2;

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
  int head = mag.readHeading();
  Serial.print(" Heading: ");
  Serial.println(head);  
if (head>indirect+error)
{
  TurnRight(150);
  }
if (head<indirect-error)
{
  TurnLeft(150);
  }
if (head>=indirect-error&&head<=indirect+error)
{
   analogWrite(E1, 0); 
   analogWrite(E2, 0);
  }
delay(100);
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
 
