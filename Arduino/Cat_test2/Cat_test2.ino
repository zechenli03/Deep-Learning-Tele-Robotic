//////////////////////////////// Sonar
#include <NewPing.h>

#define ECHO_PIN1     22 // Sonar 1 Setup. FRONT LEFT 
//Arduino pin tied to trigger pin on the ultrasonic sensor.
#define TRIGGER_PIN1  23 // Arduino pin tied to trigger pin on the ultrasonic sensor.  

#define ECHO_PIN2     24 // Sonar 2 Setup. FRONT 
#define TRIGGER_PIN2  25

#define ECHO_PIN3     26 // Sonar 3 Setup. FRONT RIGHT
#define TRIGGER_PIN3  27

#define ECHO_PIN4     28 // Sonar 4 Setup. BACK
#define TRIGGER_PIN4  29
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);
NewPing sonar3(TRIGGER_PIN3, ECHO_PIN3, MAX_DISTANCE);
NewPing sonar4(TRIGGER_PIN4, ECHO_PIN4, MAX_DISTANCE);
////////////////////////////////


const byte ledPin = 13;
const byte interruptPin1 = 3;// Left
const byte interruptPin2 = 2;// Right
int E1 = 5;
int M1 = 4; // Left
int E2 = 6;
int M2 = 7;// Right
volatile byte state1 = HIGH;
volatile byte state2 = HIGH;

int N1 = 0;// N1 N2 counts when sending move command
int N2 = 0;
int N3 = 0;// N3 N4 counts in sonar();
int N4 = 0;
int N = 0;// Distance Count
String c1 = "";

int infraredPin1 = 3; // LEFT
int infraredPin2 = 4; // RIGHT
int infraredValue1 = 0;
int infraredValue2 = 0;

void setup() {
  // initialize both serial ports:
  Serial.begin(115200);
  Serial3.begin(115200);
  for (int i = 4; i <= 7; i++) //Pin 4 to 7 are used
    pinMode(i, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin1), blink1, CHANGE);
  pinMode(interruptPin2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), blink2, CHANGE);
  digitalWrite(ledPin, state1);
  digitalWrite(ledPin, state2);
}
void loop() {
  if (Serial3.available()) {
    String inByte = Serial3.readString();
    //   Serial.println(inByte);
    N1 = 0;
    N2 = 0;
    c1 = inByte.substring(0, inByte.length() - 1);
    Serial.println(c1);
    if (c1 == "g")// Infrared sensor to adjust the car normal to plane
    {
      infraredValue1 = ((6762 / (analogRead(infraredPin1) - 9)) - 4); // convert to cm
      infraredValue2 = ((6762 / (analogRead(infraredPin2) - 9)) - 4);
      Serial.print("Value1:");
      Serial.print(infraredValue1);
      Serial.print(" Value2:");
      Serial.println(infraredValue2);
      while (infraredValue1 - infraredValue2 >= 2 || infraredValue2 - infraredValue1 >= 2)
      {
        Serial.println(infraredValue1);
        Serial.println(infraredValue2);
        infraredValue1 = ((6762 / (analogRead(infraredPin1) - 9)) - 4); // convert to cm
        infraredValue2 = ((6762 / (analogRead(infraredPin2) - 9)) - 4);
        if (infraredValue1 - infraredValue2 >= 2)
        {
          TurnRight(255);
        }
        if (infraredValue2 - infraredValue1 >= 2)
        {
          TurnLeft(255);
        }
        delay(100);
      }
      Stop();
    }

    if (c1 == "i")
    {
      CountMove("w", 255, 10);
    }
    if (c1 == "k")
    {
      CountMove("s", 255, 10);
    }
    if (c1 == "a")
    {
      TurnLeft(150);
    }
    if (c1 == "d")
    {
      TurnRight(150);
    }
    if (c1 == "x")
    {
      Stop();
    }
    if (c1 == "z")
    {
      CountTurn("a", 255, 90);
    }
    if (c1 == "c")
    {
      CountTurn("d", 255, 90);
    }
    if (c1 == "j")
    {
      CountTurn("d", 150, 180);
    }
    if (c1 == "w")
    {
      MoveForward(150);
    }
    if (c1 == "s")
    {
      MoveBackward(150);
    }
    if (c1 == "e")
    {
      CountTurn("d", 150, 10);
    }
    if (c1 == "q")
    {
      CountTurn("a", 150, 10);
    }

  }

//  MoveRandom();
  if (c1 == "w")
  {
    MoveForward(150);
  }
  if (c1 == "s")
  {
    MoveBackward(150);
  }
  if (c1 == "a")
  {
    TurnLeft(150);
  }
  if (c1 == "d")
  {
    TurnRight(150);
  }
    //  else ///////// if no input from raspberry
    //  {
    //    MoveForward(200); // Move forward
    //  }
    // read from port 0, send to port 3:
  }
  void TurnLeft(int sped)
  {
    digitalWrite(M1, LOW); //moving left
    digitalWrite(M2, HIGH);
    analogWrite(E1, sped);
    analogWrite(E2, sped);
    if (N1 % 100 || N2 % 100)
    {
//      Sonar();
    }
  }
  void TurnRight(int sped)
  {
    digitalWrite(M1, HIGH); //moving right
    digitalWrite(M2, LOW);
    analogWrite(E1, sped);
    analogWrite(E2, sped);
    if (N1 % 100 || N2 % 100)
    {
//      Sonar();
    }

  }
  void MoveForward(int sped)
  {
    digitalWrite(M1, HIGH); // moving forward
    digitalWrite(M2, HIGH);
    if (N1 % 100 || N2 % 100)
    {
 //     Sonar();
    }
    if (N1 - N2 > 50 || N2 - N1 > 50)
    {
      if (N1 - N2 > 50)
      {
        analogWrite(E1, sped * 0.8);
        analogWrite(E2, sped);
      }
      if (N2 - N1 > 50)
      {
        analogWrite(E1, sped);
        analogWrite(E2, sped * 0.8);
      }
    }
    else
    {
      analogWrite(E1, sped);
      analogWrite(E2, sped);
    }


  }
  void MoveBackward(int sped)
  {
    digitalWrite(M1, LOW); // moving backward
    digitalWrite(M2, LOW);
    if (N1 % 100 || N2 % 100)
    {
 //     Sonar();
    }
    analogWrite(E1, sped);
    analogWrite(E2, sped);
  }
  void CountTurn(String DIRC, int SPED, int ANGLE)
  {
    N1 = 0;
    N2 = 0;
    N = int(2.427 * ANGLE + 0.5);
    if (DIRC == "a")
    {
      Serial.println("Turn Right");
      while (N1 < N && N2 < N)
      {
        TurnLeft(SPED);
        //      Serial.print("N1:");
        //      Serial.print(N1);
        //      Serial.print("; N2:");
        //      Serial.print(N2);
        //      Serial.println(";");
      }
      Stop();
    }
    if (DIRC == "d")
    {
      Serial.println("Turn Right");
      while (N1 < N && N2 < N)
      {
        TurnRight(SPED);
        //      Serial.print("N1:");
        //      Serial.print(N1);
        //      Serial.print("; N2:");
        //      Serial.print(N2);
        //      Serial.println(";");

      }
      Stop();
    }
  }
  void CountMove(String DIRC, int SPED, int DISTANCE)
  {
    N1 = 0;
    N2 = 0;
    if (DIRC == "w")
    {
      Serial.println("Move Forward");
      N = int (DISTANCE * 17.94 + 0.5);
      while (N1 <= N && N2 <= N)
      {
        //      Serial.print("N1:");
        //      Serial.print(N1);
        //      Serial.print("; N2:");
        //      Serial.print(N2);
        //      Serial.println(";");
        MoveForward(SPED);
      }
      Stop();
    }
    if (DIRC == "s")
    {
      Serial.println("Move Backward");
      N = int (DISTANCE * 17.94 + 0.5);
      while (N1 <= N && N2 <= N)
      {
        //      Serial.print("N1:");
        //      Serial.print(N1);
        //      Serial.print("; N2:");
        //      Serial.print(N2);
        //      Serial.println(";");
        MoveBackward(SPED);
      }
      Stop();
    }
  }
  void Stop()
  {
    analogWrite(E1, 0);
    analogWrite(E2, 0);
  }

  void blink1() {
    N1 = N1 + 1;
    N3 = N3 + 1;
    state1 = !state1;
    digitalWrite(ledPin, state1);
  }

  void blink2() {
    N2 = N2 + 1;
    N4 = N4 + 1;
    state2 = !state2;
    digitalWrite(ledPin, state2);
  }

  void Sonar()
  {
    int BR_A = 0;
    int BR_D = 0;
    int BR_W = 0;
    int BR_S = 0;
    Serial.println("Sonar");
    //  Serial.print("Ping1:");
    //  Serial.print(sonar1.ping_cm());// Send ping, get distance in cm and print result (0 = outside set distance range)
    //  Serial.print(" Ping2:");
    //  Serial.print(sonar2.ping_cm());
    //  Serial.print(" Ping3:");
    //  Serial.print(sonar3.ping_cm());
    //  Serial.print(" Ping4:");
    //  Serial.println(sonar4.ping_cm());

    //  //////////////Front & Back
    if (sonar2.ping_cm() <= 20 && sonar2.ping_cm() > 1 && sonar4.ping_cm() <= 20 && sonar4.ping_cm() > 1)
    {
      Stop();
    }
    /////////////////////Left || Right
    else
    {
      if (sonar1.ping_cm() <= 20 && sonar1.ping_cm() > 1) //Left
      {
        CountTurn_S("d", 200, 30);
        BR_A = 1;
      }
      if (sonar3.ping_cm() <= 20 && sonar3.ping_cm() > 1) //Right
      {
        CountTurn_S("a", 200, 30);
        BR_D = 1;
      }

      ///////////Left & Front & Right
      if (sonar1.ping_cm() <= 20 && sonar2.ping_cm() > 1 && sonar3.ping_cm() <= 20 && sonar3.ping_cm() > 1)
      {
        if (sonar1.ping_cm() < sonar2.ping_cm())
        {
          CountTurn_S("d", 200, 20);
        }
        if (sonar1.ping_cm() > sonar2.ping_cm())
        {
          CountTurn_S("a", 200, 20);
        }
      }
      /////////////////Front || Back
      if (sonar2.ping_cm() <= 20 && sonar2.ping_cm() > 1) //Front
      {
        CountMove_S("s", 200, 5);
        BR_W = 1;
      }
      if (sonar4.ping_cm() <= 20 && sonar4.ping_cm() > 1) //Back
      {
        CountMove_S("w", 200, 5);
        BR_S = 1;
      }
      ///////////////////////////////REMEMBER LAST ACTION
      if (BR_A == 1 && sonar3.ping_cm() < 20 && sonar3.ping_cm())
      {
        CountMove_S("s", 200, 5);
      }
      if (BR_D == 1 && sonar1.ping_cm() < 20 && sonar1.ping_cm())
      {
        CountMove_S("s", 200, 5);
      }
    }
  }
  ///////////////////////////////////////////Move for sonar()
  void MoveForward_S(int sped)
  {
    digitalWrite(M1, HIGH); // moving backward
    digitalWrite(M2, HIGH);
    analogWrite(E1, sped);
    analogWrite(E2, sped);
  }
  void MoveBackward_S(int sped)
  {
    digitalWrite(M1, LOW);
    digitalWrite(M2, LOW);
    analogWrite(E1, sped);
    analogWrite(E2, sped);
  }

  void TurnLeft_S(int sped)
  {
    digitalWrite(M1, LOW); //moving left
    digitalWrite(M2, HIGH);
    analogWrite(E1, sped);
    analogWrite(E2, sped);
  }
  void TurnRight_S(int sped)
  {
    digitalWrite(M1, HIGH); //moving right
    digitalWrite(M2, LOW);
    analogWrite(E1, sped);
    analogWrite(E2, sped);
  }

  void CountTurn_S(String DIRC, int SPED, int ANGLE)
  {
    int RM1 = N1;
    int RM2 = N2;
    N3 = 0;
    N4 = 0;
    int DD = int(2.427 * ANGLE + 0.5);
    if (DIRC == "a")
    {
      Serial.println("Move Left_S");
      while (N3 < DD && N4 < DD)
      {
        TurnLeft_S(SPED);
        Serial.print("N3: ");
        Serial.print(N3);
        Serial.print("; ");
        Serial.print("N4: ");
        Serial.print(N4);
        Serial.println(";");
      }
      Stop();
    }
    if (DIRC == "d")
    {
      Serial.println("Move Right_S");
      while (N3 < DD && N4 < DD)
      {
        TurnRight_S(SPED);
        Serial.print("N3: ");
        Serial.print(N3);
        Serial.print("; ");
        Serial.print("N4: ");
        Serial.print(N4);
        Serial.println(";");
      }
      Stop();
    }
    N1 = RM1;
    N2 = RM2;
  }
  void CountMove_S(String DIRC, int SPED, int DISTANCE)
  {
    int RM1 = N1;
    int RM2 = N2;
    N3 = 0;
    N4 = 0;
    int DD = int (DISTANCE * 17.94 + 0.5);
    if (DIRC == "w")
    {
      Serial.println("Move Forward_S");
      while (N3 <= DD && N4 <= DD)
      {
        MoveForward_S(SPED);
        Serial.print("N3: ");
        Serial.print(N3);
        Serial.print("; ");
        Serial.print("N4: ");
        Serial.print(N4);
        Serial.println(";");
      }
      Stop();
    }
    if (DIRC == "s")
    {
      Serial.println("Move Backward_S");
      while (N3 <= DD && N4 <= DD)
      {
        MoveBackward_S(SPED);
        Serial.print("N3: ");
        Serial.print(N3);
        Serial.print("; ");
        Serial.print("N4: ");
        Serial.print(N4);
        Serial.println(";");
      }
      Stop();
    }
    N1 = RM1;
    N2 = RM2;
  }
  /////////////////////////////////////////
  void MoveRandom()
  {
    int DIRC_R = random(1, 4);
    int DIRC_D = random(5, 50);
    if (DIRC_R = 1)
    {
      CountMove("w", 200, DIRC_D);
    }
    if (DIRC_R = 2)
    {
      CountTurn("a", 200, DIRC_D);
    }
    if (DIRC_R = 3)
    {
      CountTurn("d", 200, DIRC_D);
    }
  }

