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
#include <TinyGPS++.h>
#include "EBYTE.h"
//#include <Arduino_FreeRTOS.h>
//void TaskTransmitter( void *pvParameters );
//void TaskReceiver( void *pvParameters );

#include <Wire.h> // memasukan library i2c
#include <Adafruit_SSD1306.h> // memasukan library LCD OLED SSD1306
#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);
int x;

#define PIN_RX 2
#define PIN_TX 3
#define PIN_M0 4
#define PIN_M1 5
#define PIN_AX 6

//GPS
static const int RXPin = 7, TXPin = 8;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial gps_pin(RXPin, TXPin);
//long int lat1, lon1 ;
//double lat11, lon11;
double lat2 = -7.277653, lon2 = 112.801243;


// i recommend putting this code in a .h file and including it
// from both the receiver and sender modules

// these are just dummy variables, replace with your own
struct DATA {
  unsigned long Count;
  int Bits;
  float Volts;
  float Amps;
  double latitude, longitude;
};

int Chan;
DATA MyData;

unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long previousMillis2 = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 1000;           // interval at which to blink (milliseconds)
const long interval2 = 1000;           // interval at which to blink (milliseconds)

// you will need to define the pins to create the serial port
SoftwareSerial ESerial(PIN_RX, PIN_TX);


// create the transceiver object, passing in the serial and pins
EBYTE Transceiver(&ESerial, PIN_M0, PIN_M1, PIN_AX);

void setup() {
  //  xTaskCreate(
  //    TaskTransmitter
  //    ,  "Transmitter"   // A name just for humans
  //    ,  128  // Stack size
  //    ,  NULL
  //    ,  1  // priority
  //    ,  NULL );
  //
  //  xTaskCreate(
  //    TaskReceiver
  //    ,  "Receiver"
  //    ,  128 // This stack size can be checked & adjusted by reading Highwater
  //    ,  NULL
  //    ,  2  // priority
  //    ,  NULL );

  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //ganti 0x3C dengan alamat IIC LCD kalian
  ESerial.begin(9600);
  // this init will set the pinModes for you
  Transceiver.init();
  gps_pin.begin(GPSBaud);

  // start the transceiver serial port--i have yet to get a different
  // baud rate to work--data sheet says to keep on 9600

  Serial.println("Starting Sender");

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
}

void loop() {
  // put your main code here, to run repeatedly:

  while (gps_pin.available() > 0)
    if (gps.encode(gps_pin.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }

  // measure some data and save to the structure
  MyData.latitude = gps.location.lat();
  MyData.longitude = gps.location.lng();
  //  MyData.Count++;
  //  MyData.Bits = analogRead(A0);
  //  MyData.Volts = MyData.Bits * ( 5.0 / 1024.0 );

  // You only really need this library to program these EBYTE units.
  // for writing data structures you can call write directly on the EBYTE Serial object
  //   ESerial.write((uint8_t*) &Data, PacketSize );

  // i highly suggest you send data using structures and not
  // a parsed data--i've always had a hard time getting reliable data using
  // a parsing method

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    Transceiver.SendStruct(&MyData, sizeof(MyData));
    // let the use know something was sent
    Serial.print("Sending: ");
    Serial.print(MyData.latitude, 10);
    Serial.print(", ");
    Serial.println(MyData.longitude, 10);
  }
  //DISPLAY

  unsigned long currentMillis2 = millis();

  if (currentMillis2 - previousMillis2 >= interval2) {
    // save the last time you blinked the LED
    previousMillis2 = currentMillis2;
    x = x - 1;
    display.clearDisplay(); // mengosongkan tampilan
    display.setTextSize(2); // mengosongkan tampilan
    display.setTextColor(WHITE); // set warna huruf
    //  display.setCursor(40, 0); // sett posisi tampilan
    //  display.print("FAIL"); // mengisi karakter
    display.setCursor(20, 0); // sett posisi tampilan
    display.println("SUCCESS"); // mengisi karakter

    display.setTextSize(1); // sett ukuran huruf
    display.setTextColor(WHITE); // set warna huruf
    display.setCursor(0, 16); // sett posisi tampilan
    display.print("Lat: "); // mengisi karakter
    display.println(MyData.latitude, 10);
    display.print("Long: "); // mengisi karakter
    display.println(MyData.longitude, 10);

    //  display.print("X : "); // mengisi karakter
    //  display.print(x); // mengisi karakter

    display.display(); // menampilkan karakter yang sudah disimpan
  }
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

//void TaskTransmitter(void *pvParameters)  // This is a task.
//{
//  (void) pvParameters;
//
//  for (;;) // A Task shall never return or exit.
//  {
//
//  }
//}
//
//void TaskReceiver(void *pvParameters)  // This is a task.
//{
//  (void) pvParameters;
//
//  for (;;) // A Task shall never return or exit.
//  {
//
//  }
//}

void displayInfo()
{
  //  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    //    Serial.print(gps.location.lat(), 8);
    //    Serial.print(F(","));
    //    Serial.print(gps.location.lng(), 8);
  }
  else
  {
    //    Serial.print(F("INVALID"));
  }

  //  Serial.print (HaversineDistance(gps.location.lat(), gps.location.lng(), lat2, lon2));
  //  Serial.println();
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i = strlen(sz); i < len; ++i)
    sz[i] = ' ';
  if (len > 0)
    sz[len - 1] = ' ';
  //  Serial.print(sz);
}

//FORMULA
double HaversineDistance( double y1, double x1, double y2, double x2 )
{
  double nRadius = 6370997.0; // Earth’s radius in Kilometers
  double rad = 0.0174532925199433;
  double nDLat = (y2 - y1) * rad;
  double nDLon = (x2 - x1) * rad;
  y1 *= rad;
  y2 *= rad;

  double nA =   pow( sin(nDLat / 2), 2 ) + cos(y1) * cos(y2) * pow( sin(nDLon / 2), 2 );
  double nC = 2 * atan2( sqrt(nA), sqrt( 1 - nA ));
  double nD = nRadius * nC;
  return nD;
}
