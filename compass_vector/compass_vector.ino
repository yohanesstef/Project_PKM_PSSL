#include <Wire.h>
#include <LSM303.h>
 
LSM303 compass;
 
char report[80];
 
void setup()
{
Serial.begin(9600);
Wire.begin();
compass.init();
compass.enableDefault();
compass.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
  compass.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};
}
 
void loop()
{
compass.read();
 
// snprintf(report, sizeof(report), "A: %6d %6d %6d M: %6d %6d %6d",
// compass.a.x, compass.a.y, compass.a.z,
// compass.m.x, compass.m.y, compass.m.z);
// Serial.println(report);
  float heading =compass.heading((LSM303::vector<int>){0, 0, 1});
  
  Serial.println(heading);

  delay(100);
 
// delay(500);
}