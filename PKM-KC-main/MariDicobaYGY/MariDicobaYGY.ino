#include "SPI.h"
#include "TFT_eSPI.h"
#include <TouchScreen.h>
#include <TinyGPS++.h>
#include "userInterface.h"
#include "EBYTE.h"

unsigned long previousMillis = 0;          // will store last time LED was updated
unsigned long previousMillis2 = 0;         // will store last time LED was updated
unsigned long previousMillis3 = 0;         // will store last time LED was updated
unsigned long previousMillisTransmit = 0;  // will store last time LED was updated
unsigned long previousMillisReceive = 0;   // will store last time LED was updated

// constants won't change:
const long interval = 1000;          // interval at which to blink (milliseconds)
const long interval2 = 1000;         // interval at which to blink (milliseconds)
const long interval3 = 10;           // interval at which to blink (milliseconds)
const long intervalTransmit = 2000;  // interval at which to blink (milliseconds)
const long intervalReceive = 2500;   // interval at which to blink (milliseconds)

#define PIN_RX 3  // Serial2 RX (connect this to the EBYTE Tx pin)
#define PIN_TX 1  // Serial2 TX pin (connect this to the EBYTE Rx pin)

#define PIN_M0 19  // D4 on the board (possibly pin 24)
#define PIN_M1 22  // D2 on the board (possibly called pin 22)
#define PIN_AX 21  // D15 on the board (possibly called pin 21)

EBYTE Transceiver(&Serial, PIN_M0, PIN_M1, PIN_AX);

char dataTransmit[28];
char dataReceive[29];
int ok;

#define MINPRESSURE 10
#define MAXPRESSURE 40000
TFT_eSPI tft = TFT_eSPI();                    // Invoke custom library
const int XP = 27, XM = 15, YP = 4, YM = 14;  //ID=0x9341
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
int pixel_x, pixel_y, point_z;  //Touch_getXY() updates global vars
#define PRIMARY 0xa512

//GPS
#define RXD2 16
#define TXD2 17
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
// double latGPS = -7.2777681830921255, lngGPS = 112.80178143065307;
double latGPS, lngGPS;
double latGoal = -7.277015712105961, lngGoal = 112.80220621215312;
double latRiver = -7.277668712797056, lngRiver = 112.80133162542116;
double latPos1 = -7.277703136439645, lngPos1 = 112.8020362130755;
int GPSday, GPSmonth, GPSyear, GPShour, GPSminute, GPSsecond;

//UserInterface
int menu;
int sosNav;
int distanceGoal, distanceRiver, distancePos1;
int jarakMap;

void GetTSPoint() {
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);  //restore shared pins
  pinMode(XM, OUTPUT);

  pixel_x = map(p.x, 3600, 700, 0, 240);
  pixel_y = map(p.y, 800, -2700, 0, 320);
  point_z = p.z;
}

void setup() {
  Serial.begin(9600);
  Serial.println(Transceiver.init());
  Transceiver.PrintParameters();

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  tft.init();
  tft.setRotation(0);
  menu = 1;
}

void loop() {
  while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10) {
    // Serial.println(F("No GPS detected: check wiring."));
    while (true)
      ;
  }

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // let the use know something was sent
    if (gps.location.isValid()) {
      // measure some data and save to the structure
      latGPS = gps.location.lat();
      lngGPS = gps.location.lng();
      GPSsecond = gps.time.second();
      GPSminute = gps.time.minute();
      GPShour = gps.time.hour();
      GPShour = GPShour + 7;
      if (GPShour > 23) {
        GPShour = GPShour - 24;
      } else {
        if (GPShour < 0) {
          GPShour = GPShour + 24;
        }
      }
      GPSday = gps.date.day();
      GPSmonth = gps.date.month();
      GPSyear = gps.date.year();
      // Serial.println(latGPS);
    }
  }

  unsigned long currentMillis2 = millis();

  if (currentMillis2 - previousMillis2 >= interval2) {
    // save the last time you blinked the LED
    previousMillis2 = currentMillis2;
    if(ok == 1){
      menuNavigationDisplay(20, 65);
    }
    else{
      compassDisplay(120, 160);
    }
    // userInterface();
  }

  unsigned long currentMillis3 = millis();

  if (currentMillis3 - previousMillis3 >= interval3) {
    // save the last time you blinked the LED
    previousMillis3 = currentMillis3;
    GetTSPoint();
    // Serial.print(pixel_x);
    // Serial.print(" ");
    // Serial.println(pixel_y);
  }

  unsigned long currentMillisTransmit = millis();

  if (currentMillisTransmit - previousMillisTransmit >= intervalTransmit) {
    // save the last time you blinked the LED
    previousMillisTransmit = currentMillisTransmit;
    dataTransmit[0] = 'i';
    dataTransmit[1] = 't';
    dataTransmit[2] = 's';
    memcpy(dataTransmit + 3, &latGPS, 12);
    memcpy(dataTransmit + 15, &lngGPS, 12);

    Transceiver.SendStruct(&dataTransmit, sizeof(dataTransmit));
  }

  unsigned long currentMillisReceive = millis();

  if (currentMillisReceive - previousMillisReceive >= intervalReceive) {
    // save the last time you blinked the LED
    previousMillisReceive = currentMillisReceive;
    if (Serial.available()) {
      Transceiver.GetStruct(&dataReceive, sizeof(dataReceive));
      if (dataReceive[0] == 'i' && dataReceive[1] == 't' && dataReceive[2] == 's') {
        // memcpy(&lat1, dataReceive + 3, 12);
        // memcpy(&lng1, dataReceive + 15, 12);
        memcpy(&ok, dataReceive + 3, 1);
      }
    }
  }
}

void displayInfo() {
  //  Serial.print(F("Location: "));
  if (gps.location.isValid()) {
  } else {
    //    Serial.print(F("INVALID"));
  }
}

void userInterface() {
  switch (menu) {
    case 0:
      menuNavigationDisplay(20, 65);
      break;
    case 1:
      primaryDisplay();
      break;
    case 2:
      compassDisplay(120, 160);
      break;
    case 3:
      sosDisplay();
      break;
  }
}

//==============DISPLAY 0================

void menuNavigationDisplay(uint16_t x0, uint16_t y0) {  //display 0
  tft.fillScreen(TFT_WHITE);
  drawButtonMap(x0, y0);
  drawButtonCompass(x0, y0);
  drawSOSButtonMenu(x0, y0);
  if (pixel_x >= 20 && pixel_x <= 235 && pixel_y >= 59 && pixel_y <= 103) {
    menu = 1;
  } else if (pixel_x >= 20 && pixel_x <= 235 && pixel_y >= 126 && pixel_y <= 165) {
    menu = 2;
  } else if (pixel_x >= 20 && pixel_x <= 235 && pixel_y >= 189 && pixel_y <= 237) {
    menu = 3;
  }
}

//==========DISPLAY 1===============
void primaryDisplay() {  //display 1
  distanceGoal = gps.distanceBetween(latGPS, lngGPS, latGoal, lngGoal);
  distanceRiver = gps.distanceBetween(latGPS, lngGPS, latRiver, lngRiver);
  distancePos1 = gps.distanceBetween(latGPS, lngGPS, latPos1, lngPos1);
  switch (sosNav) {
    case 0:
      tft.fillScreen(TFT_BLACK);
      tft.drawBitmap(20, 90, trees, 200, 213, TFT_TRANSPARENT);
      drawNav(10, 10);
      drawCompass(200, 280);
      drawButtonBack(10, 280);

      tft.drawBitmap(70, 65, mapFull, 72, 244, TFT_WHITE);
      jarakMap = distancePos1;
      jarakMap = map(jarakMap, 200, 0, 0, 140);
      tft.drawBitmap(118, 290 - jarakMap, point, 16, 16, TFT_RED);

      // tft.drawBitmap(85, 150, pos, 27, 24, TFT_WHITE);
      // tft.setTextColor(TFT_YELLOW);
      // tft.setTextSize(2);
      // tft.setCursor(50, 145);
      // tft.print(distancePos1);
      // tft.print("m");
      // tft.setTextSize(1);
      // tft.setCursor(50, 165);
      // tft.print("Pos 1");

      // tft.drawBitmap(30, 70, mountain, 107, 45, TFT_WHITE);
      // tft.setTextColor(TFT_YELLOW);
      // tft.setTextSize(2);
      // tft.setCursor(130, 78);
      // tft.print(distanceGoal);
      // tft.print("m");
      // tft.setTextSize(1);
      // tft.setCursor(130, 98);
      // tft.print("Puncak Gunung");

      // tft.drawBitmap(130, 205, river, 52, 34, TFT_WHITE);
      // tft.setTextColor(TFT_YELLOW);
      // tft.setTextSize(2);
      // tft.setCursor(160, 190);
      // tft.print(distanceRiver);
      // tft.print("m");
      // tft.setTextSize(1);
      // tft.setCursor(160, 210);
      // tft.print("Sungai Amazon");
      //      tft.fillRoundRect(0, 0, 30, 30, 5, TFT_BLACK);
      break;
    case 1:
      sosDisplay();
      break;
    case 2:
      compassDisplay(120, 160);
      break;
  }

  if (pixel_x >= 160 && pixel_x <= 240 && pixel_y >= 33 && pixel_y <= 53) {
    sosNav = 1;
  } else if (pixel_x >= 190 && pixel_x <= 247 && pixel_y >= 235 && pixel_y <= 289) {
    sosNav = 2;
  }

  if (pixel_x >= 14 && pixel_x <= 50 && pixel_y >= 264 && pixel_y <= 300) {
    menu = 0;
  }
}

void drawNav(uint16_t xp, uint16_t yp) {
  tft.fillRoundRect(xp, yp, 220, 60, 10, TFT_WHITE);
  tft.drawFastHLine(10, 30, 220, TFT_BLACK);

  tft.setCursor(xp + 10, yp + 5);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);

  if (GPShour < 10) {
    tft.print("0");
    tft.print(GPShour);
  } else {
    tft.print(GPShour);
  }
  tft.print(":");
  if (GPSminute < 10) {
    tft.print("0");
    tft.print(GPSminute);
  } else {
    tft.print(GPSminute);
  }
  tft.print(":");
  tft.print(GPSsecond);

  tft.setCursor(xp + 150, yp + 5);
  tft.print(GPSday);
  tft.print("/");
  tft.print(GPSmonth);
  tft.print("/");
  tft.print(GPSyear);

  tft.setCursor(20, 39);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(3);
  tft.print(distanceGoal);
  tft.print("m");
  drawSOSButton(150, 35);
}

//================DISPLAY 2===============

void compassDisplay(uint16_t x0, uint16_t y0) {  //display 2
  tft.fillScreen(TFT_BLUE);
  tft.fillCircle(x0, y0, 100, TFT_BLACK);
  tft.fillCircle(x0, y0, 95, TFT_WHITE);
  drawButtonBack(10, 10);
  if (pixel_x >= 10 && pixel_x <= 40 && pixel_y >= 8 && pixel_y <= 50) {
    menu = 0;
    sosNav = 0;
  }
}

//=================DISPLAY 3===============

void sosDisplay() {  //display 3
  tft.fillScreen(TFT_RED);
  drawButtonBack(10, 10);
  writeText(30, 84, TFT_BLACK, 3, "Sending");
  writeText(10, 112, TFT_BLACK, 3, "An Emergency");
  writeText(30, 140, TFT_BLACK, 3, "Message");
  if (pixel_x >= 10 && pixel_x <= 40 && pixel_y >= 8 && pixel_y <= 50) {
    menu = 0;
    sosNav = 0;
  }
}

//=================FUNCTION FOR USER INTERFACE==================

void drawButtonMap(uint16_t x0, uint16_t y0) {
  tft.fillRoundRect(x0, y0, 200, 50, 5, TFT_BLACK);
  tft.fillRoundRect(x0 + 2, y0 + 2, 200 - 4, 50 - 4, 5, TFT_GREEN);
  writeText(x0 + 75, y0 + 14, TFT_BLACK, 3, "MAP");
}

void drawButtonCompass(uint16_t x0, uint16_t y0) {
  tft.fillRoundRect(x0, y0 + 70, 200, 50, 5, TFT_BLACK);
  tft.fillRoundRect(x0 + 2, y0 + 72, 200 - 4, 50 - 4, 5, TFT_BLUE);
  writeText(x0 + 39, y0 + 84, TFT_BLACK, 3, "COMPASS");
}

void drawSOSButtonMenu(uint16_t x0, uint16_t y0) {
  tft.fillRoundRect(x0, y0 + 140, 200, 50, 5, TFT_BLACK);
  tft.fillRoundRect(x0 + 2, y0 + 142, 200 - 4, 50 - 4, 5, TFT_RED);
  writeText(x0 + 5, y0 + 155, TFT_BLACK, 2, "Send SOS Message");
}

void drawSOSButton(uint16_t xp, uint16_t yp) {
  tft.fillRoundRect(xp, yp, 70, 30, 15, TFT_BLACK);
  tft.fillRoundRect(xp + 2, yp + 2, 66, 26, 15, TFT_RED);
  writeText(xp + 10, yp + 4, TFT_BLACK, 3, "SOS");
}

void drawCompass(uint16_t xp, uint16_t yp) {
  tft.fillCircle(xp, yp, 30, TFT_WHITE);
}

void drawButtonBack(uint16_t xp, uint16_t yp) {
  tft.fillRoundRect(xp, yp, 30, 30, 5, TFT_BLACK);
  tft.fillRoundRect(xp + 2, yp + 2, 26, 26, 5, TFT_YELLOW);
  writeText(xp + 7, yp + 7, TFT_BLACK, 2, "<");
}

void writeText(uint16_t xp, uint16_t yp, uint16_t color, uint8_t size, char *text) {
  tft.setCursor(xp, yp);
  tft.setTextColor(color);
  tft.setTextSize(size);
  tft.print(text);
}

//=============================================================

//void menuButtonPressed(uint16_t x0, uint16_t y0) {
//  tft.fillRoundRect(x0, y0, 200, 50, 5, TFT_BLACK);
//}

//void backButtonPressed(uint16_t xp, uint16_t yp) {
//  tft.fillRoundRect(xp, yp, 30, 30, 5, TFT_BLACK);
//  delay(50);
//  drawButtonBack(xp, yp);
//  delay(50);
//}

//void sosButtonPressed(uint16_t xp, uint16_t yp) {
//  tft.fillRoundRect(xp, yp, 70, 30, 15, TFT_BLACK);
//}
