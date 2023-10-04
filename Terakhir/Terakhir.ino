#include "HardwareSerial.h"//uart
#include <TouchScreen.h>//touchscreen
#include <LSM303.h>//compas
#include <Wire.h>//I2C
#include <TFT_eSPI.h>

//GSM Object & Variable----------------------------
#define SIM_TX        16  
#define SIM_RX        17
#define SIM_BAUDRATE  9600

int _timeout;
String _buffer;
String number = "+6281299288241";

HardwareSerial SIM(2);

//TFT Object & Variable----------------------------

//Gyro GY511 variable------------------------------
LSM303 cmps;
#define GY_SCL 32
#define GY_SDA 33

//Others variable
double prev_millis[4], interval[4] = {500,500,500,500};


void setup() {
  Serial.begin(9600);

  GSM_init();

  COMPASS_init();

}

void loop() {
  //Process 1
  if(millis() > prev_millis[0] + interval[0]){
    prev_millis[0] = millis();
    GSM_routine();
    Serial.println("..");
    COMPASS_routine();
  }
  
  //Process 2

  //Process 3

  //Process 4
}