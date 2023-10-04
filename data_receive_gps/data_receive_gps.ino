#include "SoftwareSerial.h"

SoftwareSerial gps(5,6);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  gps.begin(9600);

}

void loop() {
  while(gps.available() > 0){
    byte gpsdata = gps.read();
    Serial.write(gpsdata);
  }
  // put your main code here, to run repeatedly:

}
