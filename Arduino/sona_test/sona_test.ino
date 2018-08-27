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

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
}

void loop() {
  //delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Ping1:");
  Serial.print(sonar1.ping_cm());// Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.print(" Ping2:");
  Serial.print(sonar2.ping_cm());
  Serial.print(" Ping3:");
  Serial.print(sonar3.ping_cm());
  Serial.print(" Ping4:");
  Serial.println(sonar4.ping_cm());
}
