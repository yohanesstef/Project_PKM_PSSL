/*
 * Client_1_Library_Lora_E32.ino
 *
 *  Created on: Sep 27, 2023
 *      Author: yohanes
 */
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define FREQUENCY_900
#include <LoRa_E32.h>

#define M0  2
#define M1  3
#define AUX 6

#define SCREEN_I2C_ADDR 0x3D // or 0x3C
#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RST_PIN -1      // Reset pin (-1 if not available)

Adafruit_SSD1306 display(128, 64, &Wire, OLED_RST_PIN);

uint8_t lora_txbuf[5] = {'i', 't', 's'};
unsigned long prev_millis[3];
unsigned long interval[3] = {500,500,0};
SoftwareSerial loraSerial(5,4);
LoRa_E32 loraWireless(&loraSerial, AUX, M0, M1);//baudrate otomatis 9600

void setup() {
  Serial.begin(9600);
  delay(500);
  LoraConfig();
   ResponseStatus rs = loraWireless.sendMessage("Hello, world?");
  // Check If there is some problem of successfully send
  Serial.println(rs.getResponseDescription());
}

void loop() {
  //Terima-----
  // if (millis() > prev_millis[0] + interval[0]){
  //   prev_millis[0] = millis();
  //   if (loraSerial.available()) {
  //     Serial.println("Data masuk");
  //     Serial.write(loraSerial.read());
  //   }
  //   if(loraWireless.available()>1){
  //     Serial.println("masuk loop 1");
  //     char d = loraSerial.read();
  //     Serial.println(d);
  //     ResponseContainer rc = loraWireless.receiveMessage();
  //     if(rc.status.code!=1){
  //       Serial.println(rc.status.getResponseDescription());
  //     }
  //     else{
  //       Serial.print(F("Data: "));
  //       Serial.println(rc.data);
  //     }
  //   }
  // }
  //Kirim-----
  if (millis() > prev_millis[1] + interval[1]){
    prev_millis[1] = millis();
    lora_txbuf[4]++;
    // loraSerial.println("Hello...");
    loraWireless.sendMessage(lora_txbuf,5);
    Serial.println("Ke kirim");
  }
}

//--------------------------------------------------LORA-------------------------------------------
void LoraConfig(){
  loraWireless.begin();
  ResponseStructContainer c;
  c = loraWireless.getConfiguration();
  Configuration configuration = *(Configuration*)c.data;
  Serial.println(c.status.getResponseDescription());
  Serial.println(c.status.code);
  printParameters(configuration);

  configuration.ADDL = 0x1;
  configuration.ADDH = 0x1;
  configuration.CHAN = 0x26;
    
  configuration.OPTION.fec = FEC_1_ON; //fec on agar anti interference nyala, jarak transmisi jauh
  configuration.OPTION.fixedTransmission = FT_TRANSPARENT_TRANSMISSION; //mode transmisi, defaultnya ini
  configuration.OPTION.ioDriveMode = IO_D_MODE_PUSH_PULLS_PULL_UPS; //pull up aktif
  configuration.OPTION.transmissionPower = POWER_20; //pake power yang maksimum
  configuration.OPTION.wirelessWakeupTime = WAKE_UP_250; //defaultnya ini

  configuration.SPED.airDataRate = AIR_DATA_RATE_000_03; //data rate paling rendah, jarak transmisi semakin jauh
  configuration.SPED.uartBaudRate = UART_BPS_9600;//pake baudrate yang paling max
  configuration.SPED.uartParity = MODE_00_8N1;//default

  ResponseStatus rs = loraWireless.setConfiguration(configuration, WRITE_CFG_PWR_DWN_LOSE);
  Serial.println(rs.getResponseDescription());
  Serial.println(rs.code);
  printParameters(configuration);
  c.close();
}

// void LoraReceiveRoutine(){

// }

void printParameters(struct Configuration configuration) {
    Serial.println("----------------------------------------");
 
    Serial.print(F("HEAD BIN: "));  Serial.print(configuration.HEAD, BIN);Serial.print(" ");Serial.print(configuration.HEAD, DEC);Serial.print(" ");Serial.println(configuration.HEAD, HEX);
    Serial.println(F(" "));
    Serial.print(F("AddH BIN: "));  Serial.println(configuration.ADDH, BIN);
    Serial.print(F("AddL BIN: "));  Serial.println(configuration.ADDL, BIN);
    Serial.print(F("Chan BIN: "));  Serial.print(configuration.CHAN, DEC); Serial.print(" -> "); Serial.println(configuration.getChannelDescription());
    Serial.println(F(" "));
    Serial.print(F("SpeedParityBit BIN    : "));  Serial.print(configuration.SPED.uartParity, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTParityDescription());
    Serial.print(F("SpeedUARTDataRate BIN : "));  Serial.print(configuration.SPED.uartBaudRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTBaudRate());
    Serial.print(F("SpeedAirDataRate BIN  : "));  Serial.print(configuration.SPED.airDataRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getAirDataRate());
 
    Serial.print(F("OptionTrans BIN       : "));  Serial.print(configuration.OPTION.fixedTransmission, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getFixedTransmissionDescription());
    Serial.print(F("OptionPullup BIN      : "));  Serial.print(configuration.OPTION.ioDriveMode, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getIODroveModeDescription());
    Serial.print(F("OptionWakeup BIN      : "));  Serial.print(configuration.OPTION.wirelessWakeupTime, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getWirelessWakeUPTimeDescription());
    Serial.print(F("OptionFEC BIN         : "));  Serial.print(configuration.OPTION.fec, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getFECDescription());
    Serial.print(F("OptionPower BIN       : "));  Serial.print(configuration.OPTION.transmissionPower, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getTransmissionPowerDescription());
 
    Serial.println("----------------------------------------");
 
}
void printModuleInformation(struct ModuleInformation moduleInformation) {
    Serial.println("----------------------------------------");
    Serial.print(F("HEAD BIN: "));  Serial.print(moduleInformation.HEAD, BIN);Serial.print(" ");Serial.print(moduleInformation.HEAD, DEC);Serial.print(" ");Serial.println(moduleInformation.HEAD, HEX);
 
    Serial.print(F("Freq.: "));  Serial.println(moduleInformation.frequency, HEX);
    Serial.print(F("Version  : "));  Serial.println(moduleInformation.version, HEX);
    Serial.print(F("Features : "));  Serial.println(moduleInformation.features, HEX);
    Serial.println("----------------------------------------");
}
//----

//---OLED---

//----