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
  Serial.begin(9600);
}

void loop() {
  
      while (Serial.available() > 0)  // Input
    {
       N1=0;
       N2=0;
        dist = 0;
        spd = 0;
        comdata =Serial.readString();
        dirc=comdata[0];
        spd=comdata[2]-'0';
        for (int i=4 ;i<comdata.length();i++){
             dist=dist*10+(comdata[i])-48;
          }
       
        Serial.println(dirc);
        Serial.println(spd);
        Serial.println(dist);
        N = int ((dist/21.4)*384+0.5);
    }
///////////////////////////////////////////////         
   if (dist==0||spd==0)
   {
      analogWrite(E1, 0); // speed = 0 or distance = 0 stop
      analogWrite(E2, 0);
    }
   else {
      dist=constrain (dist,0,1000);
      spd=constrain (spd,0,3);
      spd=spd*85;
    if (dirc=="w")
      {  
        MoveForward();
      }
     if (dirc=="s") 
     {
        MoveBackward();
      }
     if (dirc=="a")
     {
        TurnLeft();
      }
     if (dirc=="d")
     {
       TurnRight();
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
void TurnLeft()
{
   digitalWrite(M1,LOW);//moving left
   digitalWrite(M2,HIGH);
   analogWrite(E1, 255); 
   analogWrite(E2, 255);
  }
void TurnRight()
{  
  digitalWrite(M1,HIGH);//moving right
   digitalWrite(M2,LOW);
   analogWrite(E1, 255); 
   analogWrite(E2, 255);
  }
void MoveForward()
{
   digitalWrite(M1,LOW);// moving forward
   digitalWrite(M2,LOW);
   analogWrite(E1,spd);
   analogWrite(E2,spd);
  }
void MoveBackward()
{
   digitalWrite(M1,HIGH);// moving backward
   digitalWrite(M2,HIGH);
   analogWrite(E1,spd);
   analogWrite(E2,spd);  
  }
