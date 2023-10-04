//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Arduino Test DHT11 and LED
// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

#define DHTPIN A5     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

#define LED 12 //--> Defines an LED pin

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();

  pinMode(LED, OUTPUT);
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  if (digitalRead(LED) == 0) {
    digitalWrite(LED, HIGH);
    Serial.println("LED ON");
    Serial.println();
  } else if (digitalRead(LED) == 1) {
    digitalWrite(LED, LOW);
    Serial.println("LED OFF");
    Serial.println();
  }
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<






//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Arduino TFT Touchscreen Control LED
//----------------------------------------Include Library
#include <SPFD5408_Adafruit_GFX.h>    //--> Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> //--> Hardware-specific library
#include <SPFD5408_TouchScreen.h>
//----------------------------------------

//----------------------------------------
#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif
//----------------------------------------

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7

// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 22
//   D1 connects to digital pin 23
//   D2 connects to digital pin 24
//   D3 connects to digital pin 25
//   D4 connects to digital pin 26
//   D5 connects to digital pin 27
//   D6 connects to digital pin 28
//   D7 connects to digital pin 29

// For the Arduino Due, use digital pins 33 through 40
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 33
//   D1 connects to digital pin 34
//   D2 connects to digital pin 35
//   D3 connects to digital pin 36
//   D4 connects to digital pin 37
//   D5 connects to digital pin 38
//   D6 connects to digital pin 39
//   D7 connects to digital pin 40

//----------------------------------------Pin configuration and initialization for Touchscreen
#define YP A2 //--> must be an analog pin, use "An" notation!
#define XM A1 //--> must be an analog pin, use "An" notation!
#define YM 6  //--> can be a digital pin
#define XP 7  //--> can be a digital pin

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
//----------------------------------------

//----------------------------------------Pin configuration and initialization for LCD TFT
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
//----------------------------------------

//----------------------------------------Defines colors
// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define BLACKM  0x18E3
//----------------------------------------

//----------------------------------------The results of the x and y point calibration on the touch screen
#define TS_MINX 158
#define TS_MINY 127
#define TS_MAXX 895
#define TS_MAXY 872
//----------------------------------------

//----------------------------------------Variable for detecting touch screen when touched
#define MINPRESSURE 10
#define MAXPRESSURE 1000
//----------------------------------------

//----------------------------------------Defines an LED pin
#define LED 12
//----------------------------------------

//----------------------------------------Button location at point x and y
int BtnGreenX = 30;
int BtnGreenY = 30;
int BtnRedX = 200;
int BtnRedY = 30;
//----------------------------------------

//========================================================================VOID SETUP()
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  tft.reset();

  // *** SPFD5408 change -- Begin
//  uint16_t identifier = tft.readID();
//
//  if(identifier == 0x9325) {
//    Serial.println(F("Found ILI9325 LCD driver"));
//  } else if(identifier == 0x9328) {
//    Serial.println(F("Found ILI9328 LCD driver"));
//  } else if(identifier == 0x7575) {
//    Serial.println(F("Found HX8347G LCD driver"));
//  } else if(identifier == 0x9341) {
//    Serial.println(F("Found ILI9341 LCD driver"));
//  } else if(identifier == 0x8357) {
//    Serial.println(F("Found HX8357D LCD driver"));
//  } else {
//    Serial.print(F("Unknown LCD driver chip: "));
//    Serial.println(identifier, HEX);
//    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
//    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
//    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
//    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
//    Serial.println(F("Also if using the breakout, double-check that all wiring"));
//    Serial.println(F("matches the tutorial."));
//    return;
//  }
//
//  tft.begin(identifier);

  tft.begin(0x9341); //--> SPFD5408/ILI9341

  tft.setRotation(135); //--> Need for the Mega, please changed for your choice or rotation initial

  tft.fillScreen(WHITE); //--> TFT LCD background color

  pinMode(13, OUTPUT);  

  pinMode(LED, OUTPUT);

  DrawButtonGreen(BtnGreenX,BtnGreenY);
  DrawButtonRed(BtnRedX, BtnRedY);

  tft.setTextSize(3);
  tft.setTextColor(BLUE, WHITE);
  tft.setCursor(100, 160);
  tft.print("LED OFF");
}
//========================================================================

//========================================================================VOID LOOP()
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.height());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.width());
  
  int y_set_rotatoon_135 = map(p.x, 0, 240, 0, tft.height());
  int x_set_rotatoon_135 = map(tft.width() - p.y, 0, 320, 0, tft.width());

  //----------------------------------------Use the code lines below to display the x, y and z points on the touch screen
//  tft.setTextSize(2);
//  tft.setTextColor(RED, WHITE);
//
//  tft.setCursor(0, 55 + 70);
//  tft.print("TSPoint");
//  
//  tft.setCursor(0, 85 + 70);
//  tft.print("X:");
//  tft.print(x_set_rotatoon_135);
//  tft.print("   ");
//
//  tft.setCursor(0, 115 + 70);
//  tft.print("Y:");
//  tft.print(y_set_rotatoon_135);
//  tft.print("   ");
//
//  tft.setCursor(0, 145 + 70);
//  tft.print("Z:");
//  tft.print(p.z);
//  tft.print("   ");
  //----------------------------------------

  //----------------------------------------Conditions for detecting touch screen when touched
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    
    //----------------------------------------Condition to detect when the Green Button (Button to turn on LED) is touched and the command
    if (x_set_rotatoon_135 > BtnGreenX && x_set_rotatoon_135 < (BtnGreenX+84) && y_set_rotatoon_135 > BtnGreenY && y_set_rotatoon_135 < (BtnGreenY+84)) {
      tft.setTextSize(3);
      tft.setTextColor(BLUE, WHITE);
      tft.setCursor(100, 160);
      tft.print("LED ON ");

      digitalWrite(LED, HIGH);
      
      DrawButtonGreenPress(BtnGreenX, BtnGreenY);
      delay(100);
      DrawButtonGreen(BtnGreenX,BtnGreenY);
    }
    //----------------------------------------

    //----------------------------------------Condition to detect when the Red Button (Button to turn off LED) is touched and the command
    if (x_set_rotatoon_135 > BtnRedX && x_set_rotatoon_135 < (BtnRedX+84) && y_set_rotatoon_135 > BtnRedY && y_set_rotatoon_135 < (BtnRedY+84)) {
      tft.setTextSize(3);
      tft.setTextColor(BLUE, WHITE);
      tft.setCursor(100, 160);
      tft.print("LED OFF");

      digitalWrite(LED, LOW);
  
      DrawButtonRedPress(BtnRedX, BtnRedY);
      delay(100);
      DrawButtonRed(BtnRedX, BtnRedY);
    }
    //----------------------------------------
  }
  //----------------------------------------
}
//========================================================================

//========================================================================DrawButtonGreen(x, y)
void DrawButtonGreen(int xp, int yp) {
  tft.fillRoundRect(xp, yp, 84, 84, 10, BLACKM);
  tft.fillRoundRect(xp+2, yp+2, 80, 80, 10, GREEN);
  tft.setCursor(xp+22, yp+32);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.println("ON");
}
//========================================================================

//========================================================================DrawButtonGreenPress(x, y)
void DrawButtonGreenPress(int xp, int yp) {
  tft.fillRoundRect(xp, yp, 84, 84, 10, BLACKM);
}
//========================================================================

//========================================================================DrawButtonRed(x, y)
void DrawButtonRed(int xp, int yp) {
  tft.fillRoundRect(xp, yp, 84, 84, 10, BLACKM);
  tft.fillRoundRect(xp+2, yp+2, 80, 80, 10, RED);
  tft.setCursor(xp+18, yp+32);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.println("OFF");
}
//========================================================================

//========================================================================DrawButtonRedPress(x, y)
void DrawButtonRedPress(int xp, int yp) {
  tft.fillRoundRect(xp, yp, 84, 84, 10, BLACKM);
}
//========================================================================
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<






//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Arduino TFT Touchscreen DHT11
//----------------------------------------Include Library
#include <SPFD5408_Adafruit_GFX.h>    //--> Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> //--> Hardware-specific library

#include "DHT.h" //--> DHT library
//----------------------------------------

//----------------------------------------
#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif
//----------------------------------------

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7

// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 22
//   D1 connects to digital pin 23
//   D2 connects to digital pin 24
//   D3 connects to digital pin 25
//   D4 connects to digital pin 26
//   D5 connects to digital pin 27
//   D6 connects to digital pin 28
//   D7 connects to digital pin 29

// For the Arduino Due, use digital pins 33 through 40
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 33
//   D1 connects to digital pin 34
//   D2 connects to digital pin 35
//   D3 connects to digital pin 36
//   D4 connects to digital pin 37
//   D5 connects to digital pin 38
//   D6 connects to digital pin 39
//   D7 connects to digital pin 40

//----------------------------------------Pin configuration and initialization for LCD TFT
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
//----------------------------------------

//----------------------------------------Defines colors
// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define ORANGE  0xFD20
#define DARKORANGE  0xFB60
#define MAROON  0x7800
//----------------------------------------

//----------------------------------------DHT11 configuration and data variables
#define DHTPIN A5     
#define DHTTYPE DHT11   
DHT dht11(DHTPIN, DHTTYPE);

int h;
float t;
float f;
float hif;
float hic;
//----------------------------------------

//----------------------------------------The x and y points for the Temperature bar
int x_bar_t = 20;
int y_bar_t = 60;
//----------------------------------------

//----------------------------------------The variable to hold the conversion value from the temperature value to the value for the temperature bar
int T_to_Bar;
//----------------------------------------

//----------------------------------------Millis variable to update the temperature and humidity values
unsigned long previousMillis = 0; //--> will store last time updated
// constants won't change:
const long interval = 2000; //--> interval 
//----------------------------------------

//========================================================================VOID SETUP()
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  tft.reset();

  // *** SPFD5408 change -- Begin
//  uint16_t identifier = tft.readID();
//
//  if(identifier == 0x9325) {
//    Serial.println(F("Found ILI9325 LCD driver"));
//  } else if(identifier == 0x9328) {
//    Serial.println(F("Found ILI9328 LCD driver"));
//  } else if(identifier == 0x7575) {
//    Serial.println(F("Found HX8347G LCD driver"));
//  } else if(identifier == 0x9341) {
//    Serial.println(F("Found ILI9341 LCD driver"));
//  } else if(identifier == 0x8357) {
//    Serial.println(F("Found HX8357D LCD driver"));
//  } else {
//    Serial.print(F("Unknown LCD driver chip: "));
//    Serial.println(identifier, HEX);
//    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
//    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
//    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
//    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
//    Serial.println(F("Also if using the breakout, double-check that all wiring"));
//    Serial.println(F("matches the tutorial."));
//    return;
//  }
//
//  tft.begin(identifier);

  tft.begin(0x9341); //--> SPFD5408/ILI9341

  tft.setRotation(135); //--> Need for the Mega, please changed for your choice or rotation initial

  tft.fillScreen(WHITE); //--> TFT LCD background color

  dht11.begin();

  //----------------------------------------Initial view  
  tft.drawLine(15, 40, 300, 40, MAROON); //--> tft.drawLine(x, y, x size, y size, MAROON)
  tft.drawLine(15, 39, 300, 39, MAROON);

  tft.setTextSize(2);
  tft.setTextColor(BLUE);
  tft.setCursor(25, 10); //--> tft.setCursor(x, y)
  tft.print("Temperature & Humidity");
  
  draw_bar(x_bar_t, y_bar_t);

  tft.drawLine(190, 60, 190, 227, MAROON);
  tft.drawLine(190, 127, 300, 127, MAROON);

  tft.fillRect(202, 60, 100, 27, CYAN);
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(205, 65);
  tft.print("Humidity");

  tft.fillRect(202, 140, 100, 43, GREEN);
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(227, 145);
  tft.print("Heat");
  tft.setCursor(220, 165);
  tft.print("Index");

  GetDHT11Data();
  delay(1000);
  //----------------------------------------
}
//========================================================================

//========================================================================VOID LOOP()
void loop() {
  // put your main code here, to run repeatedly:

  //----------------------------------------Millis to update the temperature and humidity values
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    
    GetDHT11Data();
  }
  //----------------------------------------

  //----------------------------------------Temperature bar
  T_to_Bar = map(t, 0.0, 50.0, 108, 0);

  tft.fillRect(x_bar_t+8, (y_bar_t+8)+T_to_Bar, 19, 108-T_to_Bar, ORANGE);
  tft.fillRect(x_bar_t+8, y_bar_t+8, 19, T_to_Bar, WHITE);
  //----------------------------------------

  tft.setTextSize(2);
  tft.setTextColor(ORANGE, WHITE);
  tft.setCursor(75, 100);
  tft.print(t);
  if (t < 10) tft.print(" ");
  tft.setCursor(160, 100);
  tft.print((char)247);
  tft.println("C");

  
  tft.setCursor(75, 135);
  tft.print(f);
  if (f < 100) tft.print(" ");
  tft.setCursor(160, 135);
  tft.print((char)247);
  tft.println("F");

  tft.setTextSize(3);
  tft.setTextColor(CYAN, WHITE);
  tft.setCursor(205, 95);
  tft.print(h);
  tft.print(" %");

  tft.setTextSize(1);
  tft.setTextColor(GREEN, WHITE);
  tft.setCursor(205, 200);
  tft.print(hic);
  tft.print(" ");
  tft.print((char)247);
  tft.print("C");
  if (hic < 10) tft.print(" ");

  tft.setTextSize(1);
  tft.setTextColor(GREEN, WHITE);
  tft.setCursor(205, 220);
  tft.print(hif);
  tft.print(" ");
  tft.print((char)247);
  tft.print("F");
  if (hif < 100) tft.print(" "); 
}
//========================================================================

//========================================================================GetDHT11Data()
void GetDHT11Data() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht11.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht11.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  f = dht11.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  hif = dht11.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  hic = dht11.computeHeatIndex(t, h, false);
}
//========================================================================

//========================================================================draw_bar(x, y)
void draw_bar(int x_bar, int y_bar) {
  tft.fillRoundRect(x_bar, y_bar, 35, 120, 5, BLACK);
  tft.fillCircle(x_bar+17, y_bar+140, 30, BLACK);
  tft.fillRoundRect(x_bar+4, y_bar+4, 27, 120, 2, WHITE);
  tft.fillCircle(x_bar+17, y_bar+140, 25, WHITE);
  tft.fillRect(x_bar+8, y_bar+8, 19, 120, DARKORANGE);
  tft.fillCircle(x_bar+17, y_bar+140, 21, DARKORANGE);

  //tft.fillRect(41, 58, 19, 108, RED);

  tft.drawLine(x_bar+37, y_bar+8, x_bar+42, y_bar+8, RED);
  tft.setTextSize(1);
  tft.setTextColor(RED);
  tft.setCursor(x_bar+47, y_bar+4);
  tft.println("50");
  
  tft.drawLine(x_bar+37, y_bar+115, x_bar+42, y_bar+115, RED);
  tft.setCursor(x_bar+47, y_bar+111);
  tft.println("0");
}
//========================================================================
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<






//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Arduino TFT Touchscreen DHT11 & Control LED
//----------------------------------------Include Library
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>

#include "DHT.h"
//----------------------------------------

//----------------------------------------
#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif
//----------------------------------------

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7

// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 22
//   D1 connects to digital pin 23
//   D2 connects to digital pin 24
//   D3 connects to digital pin 25
//   D4 connects to digital pin 26
//   D5 connects to digital pin 27
//   D6 connects to digital pin 28
//   D7 connects to digital pin 29

// For the Arduino Due, use digital pins 33 through 40
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 33
//   D1 connects to digital pin 34
//   D2 connects to digital pin 35
//   D3 connects to digital pin 36
//   D4 connects to digital pin 37
//   D5 connects to digital pin 38
//   D6 connects to digital pin 39
//   D7 connects to digital pin 40

//----------------------------------------Pin configuration and initialization for Touchscreen
#define YP A2 //--> must be an analog pin, use "An" notation!
#define XM A1 //--> must be an analog pin, use "An" notation!
#define YM 6  //--> can be a digital pin
#define XP 7  //--> can be a digital pin

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
//----------------------------------------

//----------------------------------------Pin configuration and initialization for LCD TFT
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
//----------------------------------------

//----------------------------------------Defines colors
// Assign human-readable names to some common 16-bit color values:
#define BLACK       0x0000
#define BLUE        0x001F
#define RED         0xF800
#define GREEN       0x07E0
#define CYAN        0x07FF
#define MAGENTA     0xF81F
#define YELLOW      0xFFE0
#define WHITE       0xFFFF
#define ORANGE      0xFD20
#define DARKORANGE  0xFB60
#define MAROON      0x7800
#define BLACKM      0x18E3
//----------------------------------------

//----------------------------------------The results of the x and y point calibration on the touch screen
#define TS_MINX 158
#define TS_MINY 127
#define TS_MAXX 895
#define TS_MAXY 872
//----------------------------------------

//----------------------------------------Variable for detecting touch screen when touched
#define MINPRESSURE 10
#define MAXPRESSURE 1000
//----------------------------------------

//----------------------------------------Defines an LED pin
#define LED 12
//----------------------------------------

//----------------------------------------Button location at point x and y
int BtnGreenX = 30;
int BtnGreenY = 30;
int BtnRedX = 200;
int BtnRedY = 30;
//----------------------------------------

//----------------------------------------DHT11 configuration and data variables
#define DHTPIN A5     
#define DHTTYPE DHT11   
DHT dht11(DHTPIN, DHTTYPE);

int h;
float t;
float f;
float hif;
float hic;
//----------------------------------------

//----------------------------------------The x and y points for the Temperature bar
int x_bar_t = 20;
int y_bar_t = 60;
//----------------------------------------

//----------------------------------------The variable to hold the conversion value from the temperature value to the value for the temperature bar
int T_to_Bar;
//----------------------------------------

//----------------------------------------Menu = 0 to display the Main Menu Display, Menu = 1 to control the LED and Menu = 2 to display DHT11 sensor data
int Menu = 0;
//----------------------------------------

//----------------------------------------Variable for the x, y and z points on the touch screen
int TSPointZ;
int x_set_rotatoon_135;
int y_set_rotatoon_135;
//----------------------------------------

//----------------------------------------Millis variable to update the temperature and humidity values
unsigned long previousMillis = 0; //--> will store last time updated
// constants won't change:
const long interval = 2000; //--> interval 
//----------------------------------------

//========================================================================VOID SETUP()
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  tft.reset();

  // *** SPFD5408 change -- Begin
//  uint16_t identifier = tft.readID();
//
//  if(identifier == 0x9325) {
//    Serial.println(F("Found ILI9325 LCD driver"));
//  } else if(identifier == 0x9328) {
//    Serial.println(F("Found ILI9328 LCD driver"));
//  } else if(identifier == 0x7575) {
//    Serial.println(F("Found HX8347G LCD driver"));
//  } else if(identifier == 0x9341) {
//    Serial.println(F("Found ILI9341 LCD driver"));
//  } else if(identifier == 0x8357) {
//    Serial.println(F("Found HX8357D LCD driver"));
//  } else {
//    Serial.print(F("Unknown LCD driver chip: "));
//    Serial.println(identifier, HEX);
//    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
//    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
//    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
//    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
//    Serial.println(F("Also if using the breakout, double-check that all wiring"));
//    Serial.println(F("matches the tutorial."));
//    return;
//  }
//
//  tft.begin(identifier);

  tft.begin(0x9341); //--> SPFD5408/ILI9341

  tft.setRotation(135); //--> Need for the Mega, please changed for your choice or rotation initial

  tft.fillScreen(WHITE); //--> TFT LCD background color

  pinMode(13, OUTPUT);

  pinMode(LED, OUTPUT);

  dht11.begin();

  Menu_display();
}
//========================================================================

//========================================================================VOID LOOP()
void loop() {
  // put your main code here, to run repeatedly:

  //----------------------------------------Main Menu Display
  if (Menu == 0) {
    GetTSPoint();

    //----------------------------------------Conditions for detecting touch screen when touched
    if (TSPointZ > MINPRESSURE && TSPointZ < MAXPRESSURE) {

      //----------------------------------------Conditions for detecting when the Button for controlling the LED is touched and its command (Enter the LED controlling menu)
      if (x_set_rotatoon_135 > 17 && x_set_rotatoon_135 < (17+280) && y_set_rotatoon_135 > 90 && y_set_rotatoon_135 < (90+40)) {  
        Menu = 1;
        DrawButtonControlLEDPress();
        delay(150);
        DrawButtonControlLED();
        delay(500);
        tft.fillScreen(WHITE);
        delay(500);
        DrawButtonGreen(BtnGreenX,BtnGreenY);
        DrawButtonRed(BtnRedX, BtnRedY);
        if (digitalRead(LED) == 0) {
          tft.setTextSize(3);
          tft.setTextColor(BLUE, WHITE);
          tft.setCursor(100, 160);
          tft.print("LED OFF");
        } else if (digitalRead(LED) == 1) {
          tft.setTextSize(3);
          tft.setTextColor(BLUE, WHITE);
          tft.setCursor(100, 160);
          tft.print("LED ON");
        }
        DrawButtonBack(10, 200);
      }
      //----------------------------------------

      //----------------------------------------Condition to detect when the button to display DHT11 sensor data is touched and the command (Enter the menu displays DHT11 sensor data)
      if (x_set_rotatoon_135 > 17 && x_set_rotatoon_135 < (17+280) && y_set_rotatoon_135 > 160 && y_set_rotatoon_135 < (160+40)) {
        Menu = 2;
        DrawButtonTempHumPress();
        delay(150);
        DrawButtonTempHum();
        delay(500);
        tft.fillScreen(WHITE);
        delay(500);
        tft.drawLine(15, 40, 300, 40, MAROON);
        tft.drawLine(15, 39, 300, 39, MAROON);
      
        tft.setTextSize(2);
        tft.setTextColor(BLUE);
        tft.setCursor(40, 13);
        tft.print("Temperature & Humidity");
        
        draw_bar(x_bar_t, y_bar_t);
      
        tft.drawLine(190, 60, 190, 227, MAROON);
        tft.drawLine(190, 127, 300, 127, MAROON);
      
        tft.fillRect(202, 60, 100, 27, CYAN);
        tft.setTextSize(2);
        tft.setTextColor(WHITE);
        tft.setCursor(205, 65);
        tft.print("Humidity");
      
        tft.fillRect(202, 140, 100, 43, GREEN);
        tft.setTextSize(2);
        tft.setTextColor(WHITE);
        tft.setCursor(227, 145);
        tft.print("Heat");
        tft.setCursor(220, 165);
        tft.print("Index");

        DrawButtonBack(8, 6);
      
        GetDHT11Data();
        delay(1000);
      }
      //----------------------------------------
    }  
    //----------------------------------------
  }
  //----------------------------------------

  //----------------------------------------Menu or Mode to control the LED
  if (Menu == 1) {
    ControlTheLED();
  }
  //----------------------------------------

  //----------------------------------------Menu or Mode to display DHT11 sensor data
  if (Menu == 2) {
    ShowDHT11Data();
  }
  //----------------------------------------
}
//========================================================================

//========================================================================GetTSPoint()
void GetTSPoint() {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.height());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.width());

  y_set_rotatoon_135 = map(p.x, 0, 240, 0, tft.height());
  x_set_rotatoon_135 = map(tft.width() - p.y, 0, 320, 0, tft.width());

  TSPointZ = p.z;
}
//========================================================================

//========================================================================DrawButtonControlLED()
void DrawButtonControlLED() {
  tft.fillRoundRect(17, 90, 280, 40, 10, BLACKM);
  tft.fillRoundRect(19, 92, 276, 36, 10, GREEN);
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(65, 103);
  tft.print("Control the LED");
}
//========================================================================

//========================================================================DrawButtonControlLEDPress()
void DrawButtonControlLEDPress() {
  tft.fillRoundRect(17, 90, 280, 40, 10, BLACKM);
}
//========================================================================

//========================================================================DrawButtonTempHum()
void DrawButtonTempHum() {
  tft.fillRoundRect(17, 160, 280, 40, 10, BLACKM);
  tft.fillRoundRect(19, 162, 276, 36, 10, BLUE);
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(25, 173);
  tft.print("Temperature & Humidity");
}
//========================================================================

//========================================================================DrawButtonTempHumPress()
void DrawButtonTempHumPress() {
  tft.fillRoundRect(17, 160, 280, 40, 10, BLACKM);
}
//========================================================================

//========================================================================DrawButtonGreen(x, y)
void DrawButtonGreen(int xp, int yp) {
  tft.fillRoundRect(xp, yp, 84, 84, 10, BLACKM);
  tft.fillRoundRect(xp+2, yp+2, 80, 80, 10, GREEN);
  tft.setCursor(xp+22, yp+32);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.println("ON");
}
//========================================================================

//========================================================================DrawButtonGreenPress(x, y)
void DrawButtonGreenPress(int xp, int yp) {
  tft.fillRoundRect(xp, yp, 84, 84, 10, BLACKM);
}
//========================================================================

//========================================================================DrawButtonRed(x, y)
void DrawButtonRed(int xp, int yp) {
  tft.fillRoundRect(xp, yp, 84, 84, 10, BLACKM);
  tft.fillRoundRect(xp+2, yp+2, 80, 80, 10, RED);
  tft.setCursor(xp+18, yp+32);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.println("OFF");
}
//========================================================================

//========================================================================DrawButtonRedPress(x, y)
void DrawButtonRedPress(int xp, int yp) {
  tft.fillRoundRect(xp, yp, 84, 84, 10, BLACKM);
}
//========================================================================

//========================================================================DrawButtonBack(x, y)
void DrawButtonBack(int x_btn_back, int y_btn_back) {
  tft.fillRoundRect(x_btn_back, y_btn_back, 30, 30, 5, BLACKM);
  tft.fillRoundRect(x_btn_back+2, y_btn_back+2, 26, 26, 5, YELLOW);
  tft.setTextSize(2);
  tft.setTextColor(BLACKM);
  tft.setCursor(x_btn_back+7, y_btn_back+7);
  tft.print("<");  
}
//========================================================================

//========================================================================DrawButtonBackPress(x, y)
void DrawButtonBackPress(int x_btn_back, int y_btn_back) {
  tft.fillRoundRect(x_btn_back, y_btn_back, 30, 30, 5, BLACKM);
}
//========================================================================

//========================================================================GetDHT11Data()
void GetDHT11Data() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht11.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht11.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  f = dht11.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  hif = dht11.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  hic = dht11.computeHeatIndex(t, h, false);
}
//========================================================================

//========================================================================draw_bar (Temperature Bar)
void draw_bar(int x_bar, int y_bar) {
  tft.fillRoundRect(x_bar, y_bar, 35, 120, 5, BLACK);
  tft.fillCircle(x_bar+17, y_bar+140, 30, BLACK);
  tft.fillRoundRect(x_bar+4, y_bar+4, 27, 120, 2, WHITE);
  tft.fillCircle(x_bar+17, y_bar+140, 25, WHITE);
  tft.fillRect(x_bar+8, y_bar+8, 19, 120, DARKORANGE);
  tft.fillCircle(x_bar+17, y_bar+140, 21, DARKORANGE);

  //tft.fillRect(41, 58, 19, 108, RED);

  tft.drawLine(x_bar+37, y_bar+8, x_bar+42, y_bar+8, RED);
  tft.setTextSize(1);
  tft.setTextColor(RED);
  tft.setCursor(x_bar+47, y_bar+4);
  tft.println("50");
  
  tft.drawLine(x_bar+37, y_bar+115, x_bar+42, y_bar+115, RED);
  tft.setCursor(x_bar+47, y_bar+111);
  tft.println("0");
}
//========================================================================

//========================================================================Menu_display()
void Menu_display() {
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.setCursor(120, 43);
  tft.print("MENU");  

  DrawButtonControlLED();
  DrawButtonTempHum();
}
//========================================================================

//========================================================================ControlTheLED()
void ControlTheLED() {
  GetTSPoint();
  
  if (TSPointZ > MINPRESSURE && TSPointZ < MAXPRESSURE) {
    if (x_set_rotatoon_135 > BtnGreenX && x_set_rotatoon_135 < (BtnGreenX+84) && y_set_rotatoon_135 > BtnGreenY && y_set_rotatoon_135 < (BtnGreenY+84)) {
      tft.setTextSize(3);
      tft.setTextColor(BLUE, WHITE);
      tft.setCursor(100, 160);
      tft.print("LED ON ");

      digitalWrite(LED, HIGH);
      
      DrawButtonGreenPress(BtnGreenX, BtnGreenY);
      delay(100);
      DrawButtonGreen(BtnGreenX,BtnGreenY);
    }

    if (x_set_rotatoon_135 > BtnRedX && x_set_rotatoon_135 < (BtnRedX+84) && y_set_rotatoon_135 > BtnRedY && y_set_rotatoon_135 < (BtnRedY+84)) {
      tft.setTextSize(3);
      tft.setTextColor(BLUE, WHITE);
      tft.setCursor(100, 160);
      tft.print("LED OFF");

      digitalWrite(LED, LOW);
  
      DrawButtonRedPress(BtnRedX, BtnRedY);
      delay(100);
      DrawButtonRed(BtnRedX, BtnRedY);
    }

    if (x_set_rotatoon_135 > 10 && x_set_rotatoon_135 < (10+30) && y_set_rotatoon_135 > 200 && y_set_rotatoon_135 < (200+30)) {
      Menu = 0;
      DrawButtonBackPress(10, 200);
      delay(100);
      DrawButtonBack(10, 200);
      delay(500);
      tft.fillScreen(WHITE);
      delay(500);
      Menu_display();
    }
  }
}
//========================================================================

//========================================================================ShowDHT11Data()
void ShowDHT11Data() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    
    GetDHT11Data();
  }

  T_to_Bar = map(t, 0.0, 50.0, 108, 0);

  tft.fillRect(x_bar_t+8, (y_bar_t+8)+T_to_Bar, 19, 108-T_to_Bar, ORANGE);
  tft.fillRect(x_bar_t+8, y_bar_t+8, 19, T_to_Bar, WHITE);

  tft.setTextSize(2);
  tft.setTextColor(ORANGE, WHITE);
  tft.setCursor(75, 100);
  tft.print(t);
  if (t < 10) tft.print(" ");
  tft.setCursor(160, 100);
  tft.print((char)247);
  tft.println("C");

  
  tft.setCursor(75, 135);
  tft.print(f);
  if (f < 100) tft.print(" ");
  tft.setCursor(160, 135);
  tft.print((char)247);
  tft.println("F");

  tft.setTextSize(3);
  tft.setTextColor(CYAN, WHITE);
  tft.setCursor(205, 95);
  tft.print(h);
  tft.print(" %");

  tft.setTextSize(1);
  tft.setTextColor(GREEN, WHITE);
  tft.setCursor(205, 200);
  tft.print(hic);
  tft.print(" ");
  tft.print((char)247);
  tft.print("C");
  if (hic < 10) tft.print(" ");

  tft.setTextSize(1);
  tft.setTextColor(GREEN, WHITE);
  tft.setCursor(205, 220);
  tft.print(hif);
  tft.print(" ");
  tft.print((char)247);
  tft.print("F");
  if (hif < 100) tft.print(" "); 

  GetTSPoint();

  if (TSPointZ > MINPRESSURE && TSPointZ < MAXPRESSURE) {
    if (x_set_rotatoon_135 > 8 && x_set_rotatoon_135 < (8+30) && y_set_rotatoon_135 > 6 && y_set_rotatoon_135 < (6+30)) {
      Menu = 0;
      DrawButtonBackPress(8, 6);
      delay(100);
      DrawButtonBack(8, 6);
      delay(500);
      tft.fillScreen(WHITE);
      delay(500);
      Menu_display();
    }
  }
}
//========================================================================
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<