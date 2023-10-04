/*

  This example shows how to connect to an EBYTE transceiver
  using an Arduino Nano

  This code for for the sender


  connections
  Module      Arduino
  M0          4
  M1          5
  Rx          2 (This is the MCU Tx lined)
  Tx          3 (This is the MCU Rx line)
  Aux         6
  Vcc         3V3
  Gnd         Gnd

*/

#include <SoftwareSerial.h>
#include "EBYTE.h"
#include <Arduino_FreeRTOS.h>
void TaskTransmitter( void *pvParameters );
void TaskReceiver( void *pvParameters );

#define LORA_RX 4
#define LORA_TX 5
#define PIN_M0 2
#define PIN_M1 3
#define PIN_AX 6

// i recommend putting this code in a .h file and including it
// from both the receiver and sender modules
struct DATA {
  unsigned long Count;
  int Bits;
  float Volts;
  float Amps;
  double latitude, longitude;
};

// these are just dummy variables, replace with your own
int Chan;
DATA MyData;
unsigned long Last;

// connect to any digital pin to connect to the serial port
// don't use pin 01 and 1 as they are reserved for USB communications
SoftwareSerial ESerial(LORA_TX, LORA_RX);

// create the transceiver object, passing in the serial and pins
EBYTE Transceiver(&ESerial, PIN_M0, PIN_M1, PIN_AX);

void setup() {
  xTaskCreate(
    TaskTransmitter
    ,  "Transmitter"   // A name just for humans
    ,  128  // Stack size
    ,  NULL
    ,  1  // priority
    ,  NULL );

  xTaskCreate(
    TaskReceiver
    ,  "Receiver"
    ,  128 // This stack size can be checked & adjusted by reading Highwater
    ,  NULL
    ,  2  // priority
    ,  NULL );
}

void loop() {
  // put your main code here, to run repeatedly:
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

//ini task transmit
void TaskTransmitter(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  for (;;) // A Task shall never return or exit.
  {

  }
}


//ini task receive
void TaskReceiver(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  Serial.begin(9600);

  ESerial.begin(9600);
  Serial.println("Starting Reader");

  // this init will set the pinModes for you
  Transceiver.init();

  // all these calls are optional but shown to give examples of what you can do

  Serial.println(Transceiver.GetAirDataRate());
  Serial.println(Transceiver.GetChannel());

  Transceiver.SetAddressH(0);
  Transceiver.SetAddressL(0);
  Transceiver.SetSpeed(0b00011000);
  Transceiver.SetChannel(0b00000110);
  Transceiver.SetOptions(0b01000100);
  Transceiver.SaveParameters(PERMANENT);
  Transceiver.PrintParameters();
  for (;;) // A Task shall never return or exit.
  {
    // if the transceiver serial is available, proces incoming data
    // you can also use Transceiver.available()

    if (ESerial.available()) {

      // You only really need this library to program these EBYTE units.
      // For reading data structures, you can call readBytes directly on the EBYTE Serial object
      // ESerial.readBytes((uint8_t*)& MyData, (uint8_t) sizeof(MyData));

      // i highly suggest you send data using structures and not
      // a parsed data--i've always had a hard time getting reliable data using
      // a parsing method

      Transceiver.GetStruct(&MyData, sizeof(MyData));

      // dump out what was just received
      //    Serial.print("Count: "); Serial.println(MyData.Count);
      //    Serial.print("Bits: "); Serial.println(MyData.Bits);
      //    Serial.print("Volts: "); Serial.println(MyData.Volts);
      Serial.print("Lokasi: "); Serial.print(MyData.latitude, 10);
      Serial.print(", "); Serial.println(MyData.longitude, 10);
      // if you got data, update the checker
      //    Last = millis();

    }
    //  else {
    // if the time checker is over some prescribed amount
    // let the user know there is no incoming data
    //    if ((millis() - Last) > 1000) {
    //      Serial.println("Searching: ");
    //      Last = millis();
    //    }

    //  }
  }
}
