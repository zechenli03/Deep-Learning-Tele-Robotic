void setup() {

  Serial.begin( 9600 );
  pinMode(13,OUTPUT);

}

void loop() {
  if (Serial.available()) {
        int dy = 1000;
    while(1){
        digitalWrite(13,HIGH);
        delay(dy);
        digitalWrite(13,LOW); 
        delay(dy);
        char c = Serial.read();
        if (c=='w')
        {
          dy-=100;
          }
        if (c=='d')
        {
          dy+=100;
          }
         dy = constrain(dy,50,1500);
         Serial.println(dy);
        
        }
    }
}

