// Pi to Arduino Serial Communication Test
int N1=0;
int N2=0;
String comdata="";
String dirc="";
int dist = 0;
int spd = 0;
String c1="";
int c2=0;
int c3=0;


void setup() {
// initialize both serial ports:
	Serial.begin(9600);
	Serial3.begin(9600);
}
void loop() {
// read from port 3, send to port 0:
	if (Serial3.available()) {
    
		String inByte = Serial3.readString();
    Serial.println(inByte);
    int FirstSpace = inByte.indexOf(" ");
    c1 = inByte.substring(0,FirstSpace);
    String m1 = inByte.substring(FirstSpace+1);
    int SecondSpace = m1.indexOf(" ");
    c2 = m1.substring(0,SecondSpace).toInt();
    c3 = m1.substring(SecondSpace,m1.length()).toInt();

    dirc = c1;
    spd = c2;
    dist = c3;
    
    Serial.println(dirc);
    Serial.println(spd);
    Serial.println(dist); 
	}
// read from port 0, send to port 3:
	if (Serial.available()) {
		int inByte = Serial.read();
		Serial3.write(inByte);
	}
}
