#include <Wire.h>     
#include <Adafruit_GFX.h>  
#include <Adafruit_SSD1306.h>  

   
#define ALT 64      
#define AN 128 
    
#define OLED_RESET 4      
Adafruit_SSD1306 oled(AN, ALT, &Wire, OLED_RESET);  
#include "DHT.h"

#define DHTPIN 23     

#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Wire.begin();        
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  dht.begin();
}

void loop() {
  float t = dht.readTemperature();
  oled.clearDisplay();      
  oled.setTextColor(WHITE);  
  oled.setCursor(0, 20);    
  oled.setTextSize(3);      
  oled.print(t);  
  oled.display();    
  
}
