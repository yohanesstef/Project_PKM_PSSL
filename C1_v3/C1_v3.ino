#include <SoftwareSerial.h>
#include <EBYTE.h>

#define M0         2
#define M1         3
#define AUX        6
#define LORA_RXPIN 4 
#define LORA_TXPIN 5

typedef struct{
  uint8_t config_txbuf[];
  uint8_t config_rxbuf[];
  uint8_t param_txbuf[];
  uint8_t param_rxbuf[];
}lora_t;

typedef struct{
  uint8_t dummy[5];
}lora_data_t;

SoftwareSerial loraSerial(LORA_TXPIN, LORA_RXPIN);
EBYTE LORA(&loraSerial, M0, M1, AUX);

lora_t lora;
lora_data_t lora_data;


void setup() {
  Serial.begin(9600);
  loraSerial.begin(9600);
  Serial.println("Starting Reader...");

  LORA.init();

  Serial.println(LORA.GetAirDataRate());
  Serial.println(LORA.GetChannel());
  Serial.println(LORA.GetUARTBaudRate());

  LORA.PrintParameters();
  Serial.println("...");
}

void loop() {
  if(loraSerial.available()){
    LORA.GetStruct(&lora_data, sizeof(lora_data));
    Serial.print("Data: "); Serial.println(lora_data.dummy[0]);
  }
}

//----------------------------LORA-------------------------
void lora_init(){
  lora.config_txbuf[0] = 0xC3;
  lora.config_txbuf[1] = 0xC3;
  lora.config_txbuf[2] = 0xC3;
}

void lora_read_model_data(){

}


