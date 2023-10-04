/*
  author: yohanes
  date: 12/7/2023
  RGB565: https://rgbcolorpicker.com/565
*/
//download library touchscreen
//ganti background white
//ada tulisan
//akses touchscreen
#include <SPFD5408_Adafruit_GFX.h>
#include <SPFD5408_Adafruit_TFTLCD.h>
#include <SPFD5408_TouchScreen.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

#define YP A2 //--> must be an analog pin, use "An" notation!
#define XM A1 //--> must be an analog pin, use "An" notation!
#define YM 6  //--> can be a digital pin
#define XP 7  //--> can be a digital pin

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

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
#define PRIMARY     0xa512
#define BLUEPASTEL  0x5d7d

#define TS_MINX 158
#define TS_MINY 127
#define TS_MAXX 895
#define TS_MAXY 872

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define LED 12

//Define cordinates

int BtnGreenX = 30;
int BtnGreenY = 30;
int BtnRedX = 200;
int BtnRedY = 30;

int TSPointZ;
int x_touch;
int y_touch;

unsigned long previousMillis = 0;
const long interval = 2000;

uint8_t menu, sebelumnya;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  tft.reset();
  tft.begin(0x9341); //--> SPFD5408/ILI9341
  tft.setRotation(270); //--> Need for the Mega, please changed for your choice or rotation initial

  tft.fillScreen(PRIMARY); //--> TFT LCD background color

  pinMode(13, OUTPUT);

  pinMode(LED, OUTPUT);

  // primaryDisplay();
  // drawNav(10, 10);
  menuNavigationDisplay(20, 65);
}

void loop() {
  checkMenu();
  GetTSPoint();
  // Serial.print("x: ");
  // Serial.println(x_touch);
  // Serial.print("y: ");
  // Serial.println(y_touch);
  // Serial.print("z: ");
  // Serial.println(TSPointZ);
  // Serial.print("menu: ");
  // Serial.println(menu);
  // delay(500);
  // Serial.println(menu);
}
//End loop-------------------------
void primaryDisplay(){//display 1
  tft.fillScreen(PRIMARY);
  drawNav(10, 10);
  drawCompass(200, 280);
  drawButtonBack(10, 280);
  // tft.fillRoundRect(0, 0, 30, 30, 5, BLACKM);
}

void sosDisplay(){//display 3
  tft.fillScreen(RED);
  drawButtonBack(10, 280);
  writeText(30 , 84, BLACK, 3, "Sending");
  writeText(10, 112, BLACK, 3, "An Emergency");
  writeText(30, 140, BLACK, 3, "Message");
}

void menuNavigationDisplay(uint16_t x0, uint16_t y0){//display 0
  tft.fillScreen(WHITE);
  drawButtonMap(x0, y0);
  drawButtonCompass(x0, y0);
  drawSOSButtonMenu(x0, y0);
}

void compassDisplay(uint16_t x0, uint16_t y0){//display 2
  tft.fillScreen(BLUEPASTEL);
  tft.fillCircle(x0, y0, 100, BLACK);
  tft.fillCircle(x0, y0, 95, WHITE);
  drawButtonBack(10, 280);
}

void drawButtonMap(uint16_t x0, uint16_t y0){
  tft.fillRoundRect(x0, y0, 200, 50, 5, BLACK);
  tft.fillRoundRect(x0+2, y0+2, 200-4, 50-4, 5, GREEN);
  writeText(x0+75, y0+14, BLACK, 3, "MAP");
}

void drawButtonCompass(uint16_t x0, uint16_t y0){
  tft.fillRoundRect(x0, y0+70, 200, 50, 5, BLACK);
  tft.fillRoundRect(x0+2, y0+72, 200-4, 50-4, 5, BLUEPASTEL); 
  writeText(x0+39, y0+84, BLACK, 3, "COMPASS");
}

void drawSOSButtonMenu(uint16_t x0, uint16_t y0){
  tft.fillRoundRect(x0, y0+140, 200, 50, 5, BLACK);
  tft.fillRoundRect(x0+2, y0+142, 200-4, 50-4, 5, RED);
  writeText(x0+5, y0+155, BLACK, 2, "Send SOS Message");
}

void menuButtonPressed(uint16_t x0, uint16_t y0){
  tft.fillRoundRect(x0, y0, 200, 50, 5, BLACK);
}

void drawNav(uint16_t xp, uint16_t yp){
  tft.fillRoundRect(xp, yp, 220, 60, 10, WHITE);
  tft.drawFastHLine( 10, 30, 220, BLACK);
  writeText(20, 39, BLACK, 3, "340m");
  drawSOSButton(150, 35);
  Serial.println("Nav bar");
}

void drawCompass(uint16_t xp, uint16_t yp){    
  tft.fillCircle(xp, yp, 30, WHITE);
}

void drawButtonBack(uint16_t xp, uint16_t yp) {
  tft.fillRoundRect(xp, yp, 30, 30, 5, BLACKM);
  tft.fillRoundRect(xp+2, yp+2, 26, 26, 5, YELLOW);
  writeText(xp+7, yp+7, BLACK, 2, "<");
}

void backButtonPressed(uint16_t xp, uint16_t yp){
  tft.fillRoundRect(xp, yp, 30, 30, 5, BLACKM);
  delay(50);
  drawButtonBack(xp, yp);
  delay(50);
}

void drawSOSButton(uint16_t xp, uint16_t yp){
  tft.fillRoundRect(xp, yp, 70, 30, 15, BLACKM);
  tft.fillRoundRect(xp+2, yp+2, 66, 26, 15, RED);
  writeText(xp+10, yp+4, BLACK, 3, "SOS");
}

void sosButtonPressed(uint16_t xp, uint16_t yp){
  tft.fillRoundRect(xp, yp, 70, 30, 15, BLACKM);
}

void writeText(uint16_t xp, uint16_t yp, uint16_t color, uint8_t size, char *text){
  tft.setCursor(xp, yp);
  tft.setTextColor(color);
  tft.setTextSize(size);
  tft.print(text);
}

void GetTSPoint() {
  digitalWrite(13, HIGH);
  TSPoint p = ts.ge tPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.height());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.width());

  x_touch = p.y;
  y_touch = p.x;
  TSPointZ = p.z;
}

void checkMenu(){
  GetTSPoint();
  if (TSPointZ > MINPRESSURE && TSPointZ < MAXPRESSURE){
    if(x_touch > 9 && x_touch < 30 && y_touch > 292 && y_touch < 317 && menu != 0){
      backButtonPressed(10, 280);
      if (menu == 1){
        menuNavigationDisplay(20, 65);
        menu = 0;
        sebelumnya = 0;
      }
      else{
        switch(sebelumnya){
          case 0:
          menuNavigationDisplay(20, 65);
          menu = 0;
          break;
          case 1:
          primaryDisplay();
          menu = 1;
          break;
        }
      }
    }
    else if(x_touch > 145 && x_touch < 206 && y_touch > 43 && y_touch < 61 && menu == 1){
      sosButtonPressed(150, 35);
      delay(50);
      drawSOSButton(150, 35);
      delay(50);
      sosDisplay();
      delay(100);
      sebelumnya = 1;
      menu = 2;
    }
    else if(x_touch > 18 && x_touch < 205 && y_touch > 73 && y_touch < 117 && menu == 0){
      menuButtonPressed(20, 65);
      delay(50);
      drawButtonMap(20, 65);
      delay(50);
      primaryDisplay();
      delay(100);
      sebelumnya = 0;
      menu = 1;
    }
    else if(x_touch > 18 && x_touch < 205 && y_touch > 144 && y_touch < 190 && menu == 0){
      menuButtonPressed(20, 65+70);
      delay(50);
      drawButtonCompass(20, 65);
      delay(50);
      compassDisplay(120, 160);
      sebelumnya = 0;
      menu = 2;
    }
    else if(x_touch > 18 && x_touch < 205 && y_touch > 212 && y_touch < 262 && menu == 0){
      menuButtonPressed(20, 65+140);
      delay(50);
      drawSOSButtonMenu(20, 65);
      delay(50);
      sosDisplay();
      delay(100);
      sebelumnya = 0;
      menu = 3;
    }
  }
}






