//NO INTERNET//

#include <Wire.h>    
#include <Adafruit_GFX.h>  
#include <Adafruit_SSD1306.h>  

#define AN 128     
#define ALT 64    
#define OLED_RES 4     
Adafruit_SSD1306 oled(AN, ALT, &Wire, OLED_RES);  
#include "DHT.h"

#define DHTPIN 23    

#define DHTTYPE DHT11  

#define RELAY   18

#define BUTTON_HOURS  35
#define BUTTON_MINUTES  34

DHT dht(DHTPIN, DHTTYPE);

float t;
int flagDecimal = 0;
int hora[4] = {
  0,
  0,
  0,
  0
};

long int miliseconds;
int milisAnteriores = 0;

int maquina = 0;
#define STBY                0
#define INSERT_MINUTES         1
#define INSERT_HOURS           2
#define WAIT_INSERT_MINUTES  3
#define WAIT_INSERT_HOURS    4

void setup() {
  Wire.begin();         
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  dht.begin();
  pinMode(BUTTON_HOURS, INPUT);
  pinMode(BUTTON_MINUTES, INPUT);
  Serial.begin(115200);



}

void loop() {

  contadorTiempo();

  switch (maquina) {

    case STBY:
      if (digitalRead(BUTTON_HOURS) == LOW) {
        maquina = INSERT_HOURS;
      }

      if (digitalRead(BUTTON_MINUTES) == LOW) {
        maquina = INSERT_MINUTES;
      }
      break;

    case INSERT_HOURS:
      if (digitalRead(BUTTON_HOURS) == HIGH) {
        hora[1] += 1;
        if (hora[1] > 9) {
          hora[0] += 1;
          hora[1] = 0;
        }
        if (hora[1] > 3 && hora[0] == 2) {
          hora[0] = 0;
          hora[1] = 0;
          hora[2] = 0;
          hora[3] = 0;
        }
        maquina = STBY;
      }
      break;

    case INSERT_MINUTES:
      if (digitalRead(BUTTON_MINUTES) == HIGH) {
        hora[3] += 1;
        if (hora[3] > 9) {
          hora[2] += 1;
          hora[3] = 0;
        }

        if (hora[2] > 5) {
          hora[1] += 1;
          hora[2] = 0;
        }
        maquina = STBY;
      }
  }

  pantalla1();
}

void contadorTiempo() {
  miliseconds = millis();



  if (miliseconds - milisAnteriores > 60000) {
    hora[3] += 1;

    if (hora[3] > 9) {
      hora[2] += 1;
      hora[3] = 0;
    }

    if (hora[2] > 5) {
      hora[1] += 1;
      hora[2] = 0;
    }

    if (hora[1] > 9) {
      hora[0] += 1;
      hora[1] = 0;
    }
    if (hora[1] > 3 && hora[0] == 2) {
      hora[0] = 0;
      hora[1] = 0;
      hora[2] = 0;
      hora[3] = 0;
    }



    milisAnteriores = miliseconds;
  }
}