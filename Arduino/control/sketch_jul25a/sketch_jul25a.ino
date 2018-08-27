#include <NewPing.h>
#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 200
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

const byte ledPin = 13;
const byte interruptPin2 = 3;// Left
const byte interruptPin1 = 2;// Right
int E1 = 5;  
int M1 = 4; // Right
int E2 = 6;                      
int M2 = 7;// Left  
volatile byte state1 = HIGH;
volatile byte state2 = HIGH;

String comdata = "";  
int N1 = 0;
int N2 = 0;
int N = 0;
String dirc = "";
int spd = 0;
float dist = 0;
float Blkdist = 0;

void setup() {
  for (int i = 4; i <= 7; i++) //Pin 4 to 7 are used
      pinMode(i, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin1), blink1, CHANGE);
  pinMode(interruptPin2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), blink2, CHANGE);
  digitalWrite(ledPin, state1);
  digitalWrite(ledPin, state2);
  Serial.begin(115200);
  Serial3.begin(115200);
}

void loop() {
      while (Serial3.available() > 0)  // Input
    {
        N1=0;
        N2=0;
        dist = 0;
        spd = 0;
        comdata =Serial3.readString();
        dirc=comdata[0];
        spd=comdata[2]-'0';
        for (int i=4 ;i<comdata.length();i++){
             dist=dist*10+(comdata[i])-48;
          }
        dist=constrain (dist,0,1000);
        spd=constrain (spd,0,3);
        /*Serial.println(dirc);
        Serial.println(spd);
        Serial.println(dist);*/
        N = int ((dist/21.4)*384+0.5);
        spd=spd*85;
        delay(3000);
    }
///////////////////////////////////////////////     
   if (dist==0||spd==0)
   {
      analogWrite(E1, 0); // speed = 0 or distance = 0 stop
      analogWrite(E2, 0);
    }
   else {
    if (dirc=="w")
      { 
        if (sonar.ping_cm()<=5)
        {
          TurnLeft(150);
          Serial.println(sonar.ping_cm());
          } 
        else {
        MoveForward(spd);
        }
        
      }
     if (dirc=="s") 
     {
        MoveBackward(spd);
      }
     if (dirc=="a")
     {
        TurnLeft(spd);
      }
     if (dirc=="d")
     {
       TurnRight(spd);
      }
        Serial.print("Left wheel: ");
        Serial.print((N2/384.0)*21.4);
        Serial.println("cm;");
        Serial.print("Right wheel: ");
        Serial.print((N1/384.0)*21.4);
        Serial.println("cm;");
    }  
}
///////////////////////////////////////////////////////////////
void blink1() {
  N1=N1+1;
  if (N1>=N)
  {
    spd = 0;
    analogWrite(E1,spd);
    N1=0;
    }
  state1 = !state1;
  digitalWrite(ledPin, state1);
}

void blink2() {
  N2=N2+1;
  if (N2>=N)
  {
    spd = 0;
    analogWrite(E2,spd);
    N2=0;
    }
  state2 = !state2;
  digitalWrite(ledPin, state2);
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
 
