#include <Wire.h>     
#include <Adafruit_GFX.h>  
#include <Adafruit_SSD1306.h>   

#define ALT 64 
#define ANC 128     


#define OLED_RESET 4      
Adafruit_SSD1306 oled(ANC, ALT, &Wire, OLED_RESET);  

#include "DHT.h"

#define DHTPIN 23    

#define DHTTYPE DHT11   

#define RELAY   18

#define BTN_SUB  35
#define BTN_BAJ  34

DHT dht(DHTPIN, DHTTYPE);

float t;

float tempElegida = 31;

int maquina = 0;
#define TEMP_INF   0
#define TEMP_SUP   1
#define CONFIG          2
#define ESPERA_CONFIG   3
#define ESPERA_SUB    4
#define ESPERA_BAJ    5
#define ESPERA_CONFIG2  6

void setup() {
  Wire.begin();         
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  dht.begin();
  pinMode(RELAY, OUTPUT);
  pinMode(BTN_SUB, INPUT);
  pinMode(BTN_BAJ, INPUT);



}

void loop() {

  maquinaDeEstados();
 
}

void maquinaDeEstados() {

  switch (maquina) {
    case TEMP_INF:
      if (t > tempElegida) {    
        digitalWrite(RELAY, HIGH);
        maquina = TEMP_SUP;
      }
      if (digitalRead(BTN_SUB) == LOW && digitalRead(BTN_BAJ) == LOW) {
        seleccionarTemp();
        maquina = ESPERA_CONFIG;
      }   
      pantalla1();
      break;
    case TEMP_SUP:
      if (t < tempElegida) {
        digitalWrite(RELAY, LOW); 
        maquina = TEMP_INF;
      }
      if (digitalRead(BTN_SUB) == LOW && digitalRead(BTN_BAJ) == LOW) {
        seleccionarTemp();
        maquina = ESPERA_CONFIG;
      }
      pantalla1();
      break;

    case ESPERA_CONFIG:
      if (digitalRead(BTN_SUB) == HIGH && digitalRead(BTN_BAJ) == HIGH) {
        maquina = CONFIG;
      }
      break;

    case CONFIG:
      if (digitalRead(BTN_SUB) == LOW) {
        tempElegida += 0.1;
        maquina = ESPERA_SUB;   
      }
      if (digitalRead(BTN_BAJ) == LOW) {
        tempElegida -= 0.1;
        maquina = ESPERA_BAJ;
      }
      if (digitalRead(BTN_SUB) == LOW && digitalRead(BTN_BAJ) == LOW) {
        pantalla1();
        maquina = ESPERA_CONFIG2;
      }
      seleccionarTemp();
      break;

    case ESPERA_CONFIG2:
      t = dht.readTemperature();
      if (digitalRead(BTN_SUB) == HIGH && digitalRead(BTN_BAJ) == HIGH) {
        if (t < tempElegida) {
          maquina = TEMP_INF;
        }
        if (t > tempElegida) {
          maquina = TEMP_SUP;
        }
      }
      break;

    case ESPERA_SUB:
      if (digitalRead(BTN_SUB) == HIGH) {
        maquina = CONFIG;
      }
      break;

    case ESPERA_BAJ:
      if (digitalRead(BTN_BAJ) == HIGH) {
        maquina = CONFIG;
      }
      break;
  }
}
