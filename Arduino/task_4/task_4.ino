const byte ledPin = 13;
const byte interruptPin1 = 2;
const byte interruptPin2 = 3;
volatile byte state1 = HIGH;
volatile byte state2 = HIGH;
int N1 = 0;
int N2 = 0;
void setup() {
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
  Serial.print("Working hard...");
  delay(1000);       
  Serial.print("Left distance = ");
  float distance1 = (N1/384.0)*21.4;
  Serial.print(distance1);
  Serial.print(" cm;");
  Serial.print("Right distance = ");
  float distance2 = (N2/384.0)*21.4;
  Serial.print(distance2);
  Serial.println(" cm;");
  
}

void blink1() {
  N1=N1+1;
  state1 = !state1;
  digitalWrite(ledPin, state1);
}

void blink2() {
  N2=N2+1;
  state2 = !state2;
  digitalWrite(ledPin, state2);
}
