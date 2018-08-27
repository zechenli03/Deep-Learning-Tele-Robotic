const byte ledPin = 13;
const byte interruptPin = 2;
volatile int count = 0;

void setup() {

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
  
  Serial.begin(9600);
}

void loop() {
  Serial.print("Working hard...");
  delay(1000);       
  Serial.print("done. Count = ");
  Serial.println(count);
}

void blink() {
  count++;
}
