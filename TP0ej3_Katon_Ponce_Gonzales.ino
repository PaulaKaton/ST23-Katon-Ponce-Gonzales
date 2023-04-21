
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
#define UMBRAL  31

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Wire.begin();         
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  dht.begin();
  pinMode(RELAY, OUTPUT);
}

void loop() {
  float t = dht.readTemperature();
  oled.clearDisplay();      
  oled.setTextColor(WHITE);   
  oled.setCursor(0, 20);     
  oled.setTextSize(3);      
  oled.print(t);  
  oled.display();     

  if(t > UMBRAL){
    digitalWrite(RELAY, HIGH);
  }else{
    digitalWrite(RELAY, LOW);
  }

}
