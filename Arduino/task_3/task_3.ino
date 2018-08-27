//Note
//Pin 4 = Direction control for Motor 2
//Pin 5 = PWM control for Motor 2
//Pin 6 = PWM control for Motor 1
//Pin 7 = Direction control for Motor 1
int E1 = 5;  
int M1 = 4; 
int E2 = 6;                      
int M2 = 7;  

void setup() {

  Serial.begin( 9600 );
  for (int i = 4; i <= 7; i++) //Pin 4 to 7 are used
      pinMode(i, OUTPUT);

}

void loop() {

  if (Serial.available()) {
        char input = Serial.read();
        switch(input){
            case 'x':
                analogWrite(E1, 0); //0 power == stop
                analogWrite(E2, 0);
                break;
            case 'w':
                digitalWrite(M1,LOW);// moving forward
                digitalWrite(M2,LOW);
                analogWrite(E1, 255); 
                analogWrite(E2, 255);
                break;
           case 's':
                digitalWrite(M1,HIGH);//moving backward
                digitalWrite(M2,HIGH);
                analogWrite(E1, 255); //255 = max power
                analogWrite(E2, 255);
                break;
           case 'a':
                digitalWrite(M1,LOW);//moving left
                digitalWrite(M2,HIGH);
                analogWrite(E1, 255); 
                analogWrite(E2, 255);
                break;
            case 'd':
                digitalWrite(M1,HIGH);//moving right
                digitalWrite(M2,LOW);
                analogWrite(E1, 255);
                analogWrite(E2, 255);
                break;

        }
     }
}


