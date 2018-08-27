// Pi to Arduino Serial Communication Test
#include <SparkFun_MAG3110.h>
#include <NewPing.h>
#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 200
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

const byte ledPin = 13;
const byte interruptPin1 = 3;// Left
const byte interruptPin2 = 2;// Right
int E1 = 5;  
int M1 = 4; // Left
int E2 = 6;                      
int M2 = 7;// Right  
volatile byte state1 = HIGH;
volatile byte state2 = HIGH;
int N1 = 0;
int N2 = 0;
int N = 0;
String dirc = "";
int spd = 0;
float dist = 0;
String comdata = "";
String c1 = "";
int c2=0;
int c3=0;
float TurnRate = 0;
int HeadValue = 0;
float error = 2;
int iniHead = 0;
int x,y,z;
MAG3110 mag = MAG3110(); //Instantiate MAG3110


void setup() {
// initialize both serial ports:
  Serial.begin(115200);
  Serial3.begin(115200);
  mag.initialize();
  for (int i = 4; i <= 7; i++) //Pin 4 to 7 are used
      pinMode(i, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin1), blink1, CHANGE);
  pinMode(interruptPin2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), blink2, CHANGE);
  digitalWrite(ledPin, state1);
  digitalWrite(ledPin, state2);
//delay(5000);
//  MagSetup();
  HeadValue = head();
  mag.readMag(&x, &y, &z);
  iniHead = y;
  blink1();
  delay(1000);
  blink1();
  Serial.println("Arduino has set up!"); 
}


void loop() {
// read from port 3, send to port 0:

  if (Serial3.available()) {
    String inByte = Serial3.readString();
    Serial.println(inByte);
    HeadValue = mag.readHeading();
/////////////////////////////////////
    if(inByte=='Y'){
     inByte='K';
     Serial3.write("K");
     Serial.print("inByte:");
     Serial.println(inByte);
    }
/////////////////////////////////////
    else
    { 
      N1=0;
      N2=0;
 //     int FirstSpace = inByte.indexOf(" ");
      c1 = inByte.substring(0,inByte.length()-1);
 //     String m1 = inByte.substring(FirstSpace+1);
 //     int SecondSpace = m1.indexOf(" ");
 //     c2 = m1.substring(0,SecondSpace).toInt();
 //     c3 = m1.substring(SecondSpace,m1.length()).toInt();

      dirc = c1;
 //     spd = c2;
 //     dist = c3;
    
      Serial.println(dirc);
//      Serial.println(spd);
//      Serial.println(dist); 

      N = int ((dist/21.4)*384+0.5);
      dist=constrain (dist,0,1000);
      spd=constrain (spd,0,3);
      spd=spd*85;
/*   if (dist==0||spd==0)
   {
      analogWrite(E1, 0); // speed = 0 or distance = 0 stop
      analogWrite(E2, 0);
    }*/
/////////////////////////////////////////////Move 20cm

    if (dirc=="e")
      { 
        spd = 255;
        N = int ((5/21.4)*384+0.5);
        MoveForward(spd);
        while (N1<=N&&N2<=N)
        {
          Serial.println(N1);
          }
         Stop();
       }
     if (dirc=="sss") 
     {  
        spd=255;
        N = int ((5/21.4)*384+0.5);
        MoveBackward(spd);
         while (N1<=N&&N2<=N)
        {
          Serial.println(N1);
          }
         Stop();
      }
/////////////////////////////////////Move 50cm
     if (dirc=="ww")
      { 
        spd = 255;
        N = int ((50/21.4)*384+0.5);
        MoveForward(spd);
        while (N1<=N&&N2<=N)
        {
          Serial.println(N1);
          }
         Stop();
       }
     if (dirc=="ss") 
     {  
        spd=255;
        N = int ((50/21.4)*384+0.5);
        MoveBackward(spd);
         while (N1<=N&&N2<=N)
        {
          Serial.println(N1);
          }
         Stop();
      }
///////////////////////////////////Move no stop
     if (dirc=="w")
     {
       MoveForward(255);
      }
     if (dirc=="s")
     {
      MoveBackward(255);
      }
////////////////////////////////////Stop      
      if (dirc=="x")
      {
        Stop();
        }
//////////////////////////////////////////////////////  Turn Left  
     if (dirc=="zzzz")
     {  
        spd = 255;
        HeadValue = head(); 
        TurnRate = HeadValue + 5; // Turning angle 
        if (TurnRate>360)
        {
          TurnRate = TurnRate - 360;
         }
         if (TurnRate<0)
         {
          TurnRate = TurnRate + 360;
          }
        if ((TurnRate+error)>=360)
        { 
          int uplimit = TurnRate+error-360;
          int downlimit = TurnRate-error;
          while (HeadValue>=uplimit&&HeadValue<=downlimit)
          {
            TurnLeft(spd);
            Serial.print("Head: ");
            Serial.print(HeadValue);
            Serial.print("; Ideal direction: ");
            Serial.println(TurnRate);
            HeadValue = head();
            
            }
          if ((TurnRate-error)<=0)
          {
            int uplimit = TurnRate+error;
            int downlimit = TurnRate-error+360;
            while (HeadValue>=uplimit&&HeadValue<=downlimit)
            {
            TurnLeft(spd);
            Serial.print("Head: ");
            Serial.print(HeadValue);
            Serial.print("; Ideal direction: ");
            Serial.println(TurnRate);
            HeadValue = head();
            }
           }
          }
         else
         { 
            int uplimit = TurnRate+error;
            int downlimit = TurnRate-error;
            while (HeadValue>=uplimit||HeadValue<=downlimit)
            {
              TurnLeft(spd);
              Serial.print("Head: ");
              Serial.print(HeadValue);
              Serial.print("; Ideal direction: ");
              Serial.println(TurnRate);
              HeadValue = head();
            } 
          }
          Stop();

      }
      
    /* if (dirc=="zz")
     {  
        spd = 255;
        HeadValue = head(); 
        TurnRate = HeadValue + 20; // Turning angle
        if (TurnRate>360)
        {
          TurnRate = TurnRate - 360;
         }
         if (TurnRate<0)
         {
          TurnRate = TurnRate + 360;
          }
        if ((TurnRate+error)>=360)
        { 
          int uplimit = TurnRate+error-360;
          int downlimit = TurnRate-error;
          while (HeadValue>=uplimit&&HeadValue<=downlimit)
          {
            TurnLeft(spd);
            Serial.print("Head: ");
            Serial.print(HeadValue);
            Serial.print("; Ideal direction: ");
            Serial.println(TurnRate);
            HeadValue = head();
            
            }
          if ((TurnRate-error)<=0)
          {
            int uplimit = TurnRate+error;
            int downlimit = TurnRate-error+360;
            while (HeadValue>=uplimit&&HeadValue<=downlimit)
            {
            TurnLeft(spd);
            Serial.print("Head: ");
            Serial.print(HeadValue);
            Serial.print("; Ideal direction: ");
            Serial.println(TurnRate);
            HeadValue = head();
            }
           }
          }
         else
         { 
            int uplimit = TurnRate+error;
            int downlimit = TurnRate-error;
            while (HeadValue>=uplimit||HeadValue<=downlimit)
            {
              TurnLeft(spd);
              Serial.print("Head: ");
              Serial.print(HeadValue);
              Serial.print("; Ideal direction: ");
              Serial.println(TurnRate);
              HeadValue = head();
            } 
          }
          Stop();
      } 
      */
      if (dirc=="zzz")
      {
        TurnLeft(150);
        }     
////////////////////////////////////////////////////// Turn Right
     if (dirc=="cccc")
     {  
        spd = 255;
        HeadValue = head(); 
        TurnRate = HeadValue - 5; // Turning angle
        if (TurnRate<360)
        {
          TurnRate = TurnRate + 360;
         }
        if (TurnRate>360)
        {
          TurnRate = TurnRate - 360;
          }
        if ((TurnRate+error)>=360)
        { 
          int uplimit = TurnRate+error-360;
          int downlimit = TurnRate-error;
          while (HeadValue>=uplimit&&HeadValue<=downlimit)
          {
            TurnRight(spd);
            Serial.print("Head: ");
            Serial.print(HeadValue);
            Serial.print("; Ideal direction: ");
            Serial.println(TurnRate);
            HeadValue = head();
            
            }
          if ((TurnRate-error)<=0)
          {
            int uplimit = TurnRate+error;
            int downlimit = TurnRate-error+360;
            while (HeadValue>=uplimit&&HeadValue<=downlimit)
            {
            TurnRight(spd);
            Serial.print("Head: ");
            Serial.print(HeadValue);
            Serial.print("; Ideal direction: ");
            Serial.println(TurnRate);
            HeadValue = head();
            }
           }
          }
         else
         { 
            int uplimit = TurnRate+error;
            int downlimit = TurnRate-error;
            while (HeadValue>=uplimit||HeadValue<=downlimit)
            {
              TurnRight(spd);
              Serial.print("Head: ");
              Serial.print(HeadValue);
              Serial.print("; Ideal direction: ");
              Serial.println(TurnRate);
              HeadValue = head();
            } 
          }
          Stop();
      }

     if (dirc=="cc")
     {  
        spd = 255;
        HeadValue = head(); 
        TurnRate = HeadValue - 20; // Turning angle
        if (TurnRate<360)
        {
          TurnRate = TurnRate + 360;
         }
        if (TurnRate>360)
        {
          TurnRate = TurnRate - 360;
          }
        if ((TurnRate+error)>=360)
        { 
          int uplimit = TurnRate+error-360;
          int downlimit = TurnRate-error;
          while (HeadValue>=uplimit&&HeadValue<=downlimit)
          {
            TurnRight(spd);
            Serial.print("Head: ");
            Serial.print(HeadValue);
            Serial.print("; Ideal direction: ");
            Serial.println(TurnRate);
            HeadValue = head();
            
            }
          if ((TurnRate-error)<=0)
          {
            int uplimit = TurnRate+error;
            int downlimit = TurnRate-error+360;
            while (HeadValue>=uplimit&&HeadValue<=downlimit)
            {
            TurnRight(spd);
            Serial.print("Head: ");
            Serial.print(HeadValue);
            Serial.print("; Ideal direction: ");
            Serial.println(TurnRate);
            HeadValue = head();
            }
           }
          }
         else
         { 
            int uplimit = TurnRate+error;
            int downlimit = TurnRate-error;
            while (HeadValue>=uplimit||HeadValue<=downlimit)
            {
              TurnRight(spd);
              Serial.print("Head: ");
              Serial.print(HeadValue);
              Serial.print("; Ideal direction: ");
              Serial.println(TurnRate);
              HeadValue = head();
            } 
          }
          Stop();
      }
      /////// Turning no stop
     if (dirc=="ccc")
      {
        TurnLeft(150);
        }
 /////////////////////////////////////////////////////////////
     if (dirc == "a")
     {
      CountTurn("a",150,10);
      }
     if (dirc == "d")
     {
      CountTurn("d",150,10);
      }
     if (dirc == "z")
     {
      CountTurn("a",255,25);
      }
     if (dirc == "c")
     {
      CountTurn("d",255,25);
      }
     if (dirc == "r")
     {
      MoveForward(255);
      }
    }
  }   
    if (dirc=="r")
    {
      mag.readMag(&x, &y, &z);
      HeadValue=y;
      Serial.print(HeadValue);
      Serial.print("; ");
      Serial.println(iniHead);
      if (HeadValue>iniHead+9||HeadValue<iniHead-9)
      {
      if (HeadValue>iniHead+9)
      {
        TurnRight(120);
        }
       if (HeadValue<iniHead-9)
       {
        TurnLeft(120);
        }
      }
      else
      {
        MoveForward(255);
        }
      }
}
 
// read from port 0, send to port 3:






/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
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
   digitalWrite(M1,HIGH);// moving forward
   digitalWrite(M2,HIGH);
   analogWrite(E1,sped);
   analogWrite(E2,sped);
  }
void MoveBackward(int sped)
{
   digitalWrite(M1,LOW);// moving backward
   digitalWrite(M2,LOW);
   analogWrite(E1,sped);
   analogWrite(E2,sped);  
  }

void Stop()
{
   analogWrite(E1,0);
   analogWrite(E2,0);
  }
  
void blink1() {
  N1=N1+1;
/*  if (N1>=N)
  {
    spd = 0;
    analogWrite(E1,spd);
    N1=0;
    }*/
  state1 = !state1;
  digitalWrite(ledPin, state1);
}

void blink2() {
  N2=N2+1;
/*  if (N2>=N)
  {
    spd = 0;
    analogWrite(E2,spd);
    N2=0;
    }*/
  state2 = !state2;
  digitalWrite(ledPin, state2);
}

void MagSetup()
{
  while (!mag.isCalibrated())
  { 
   //TurnLeft(255); // Turn round to calibrate; 
  int x, y, z;

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
  mag.readMag(&x, &y, &z);

  Serial.print("X: ");
  Serial.print(x);
  Serial.print(", Y: ");
  Serial.print(y);
  Serial.print(", Z: ");
  Serial.println(z);

  Serial.print("Heading: ");
  Serial.println(mag.readHeading());

  Serial.println("--------");
  
  delay(100);
  }
  Stop();
}
int head()
{
  if (mag.readHeading()<0)
  {
    return mag.readHeading()+360;
    }
   else
   {
    return mag.readHeading();
    }
}

void MagTurn(String DIRC, int SPED, int ANGLE)
{
  
  if (DIRC == "a")
  {
        error = 2;
        HeadValue = head(); 
        TurnRate = HeadValue + ANGLE;
        if (TurnRate>360)
        {
          TurnRate = TurnRate - 360;
         }
         if (TurnRate<0)
         {
          TurnRate = TurnRate + 360;
          }
        if ((TurnRate+error)>=360)
        { 
          int uplimit = TurnRate+error-360;
          int downlimit = TurnRate-error;
          while (HeadValue>=uplimit&&HeadValue<=downlimit)
          {
            TurnLeft(SPED);
            Serial.print("Head: ");
            Serial.print(HeadValue);
            Serial.print("; Ideal direction: ");
            Serial.println(TurnRate);
            HeadValue = head();
            
            }
          if ((TurnRate-error)<=0)
          {
            int uplimit = TurnRate+error;
            int downlimit = TurnRate-error+360;
            while (HeadValue>=uplimit&&HeadValue<=downlimit)
            {
            TurnLeft(SPED);
            Serial.print("Head: ");
            Serial.print(HeadValue);
            Serial.print("; Ideal direction: ");
            Serial.println(TurnRate);
            HeadValue = head();
            }
           }
          }
         else
         { 
            int uplimit = TurnRate+error;
            int downlimit = TurnRate-error;
            while (HeadValue>=uplimit||HeadValue<=downlimit)
            {
              TurnLeft(SPED);
              Serial.print("Head: ");
              Serial.print(HeadValue);
              Serial.print("; Ideal direction: ");
              Serial.println(TurnRate);
              HeadValue = head();
            } 
          }
          Stop();
    }
  if (DIRC == "d")
  {
        error = 2;
        HeadValue = head(); 
        TurnRate = HeadValue - ANGLE;
        if (TurnRate<360)
        {
          TurnRate = TurnRate + 360;
         }
        if (TurnRate>360)
        {
          TurnRate = TurnRate - 360;
          }
        if ((TurnRate+error)>=360)
        { 
          int uplimit = TurnRate+error-360;
          int downlimit = TurnRate-error;
          while (HeadValue>=uplimit&&HeadValue<=downlimit)
          {
            TurnRight(SPED);
            Serial.print("Head: ");
            Serial.print(HeadValue);
            Serial.print("; Ideal direction: ");
            Serial.println(TurnRate);
            HeadValue = head();
            
            }
          if ((TurnRate-error)<=0)
          {
            int uplimit = TurnRate+error;
            int downlimit = TurnRate-error+360;
            while (HeadValue>=uplimit&&HeadValue<=downlimit)
            {
            TurnRight(SPED);
            Serial.print("Head: ");
            Serial.print(HeadValue);
            Serial.print("; Ideal direction: ");
            Serial.println(TurnRate);
            HeadValue = head();
            }
           }
          }
         else
         { 
            int uplimit = TurnRate+error;
            int downlimit = TurnRate-error;
            while (HeadValue>=uplimit||HeadValue<=downlimit)
            {
              TurnRight(SPED);
              Serial.print("Head: ");
              Serial.print(HeadValue);
              Serial.print("; Ideal direction: ");
              Serial.println(TurnRate);
              HeadValue = head();
            } 
          }
          Stop();    
    }
  }

void CountTurn(String DIRC, int SPED, int ANGLE)
{
  N1 = 0;
  N2 = 0;
  N = int(2.427*ANGLE+0.5);
  if (DIRC == "a")
  {
    while(N1<N&&N2<N)
    {
      TurnLeft(SPED);
      Serial.print("N1:");
      Serial.print(N1);
      Serial.print("; N2:");
      Serial.print(N2);
      Serial.println(";");
      }
     Stop();
    }
  if (DIRC == "d")
  {
    while (N1<N&&N2<N)
    {
      TurnRight(SPED);
      Serial.print("N1:");
      Serial.print(N1);
      Serial.print("; N2:");
      Serial.print(N2);
      Serial.println(";");
      }
    Stop();
    }
  }
