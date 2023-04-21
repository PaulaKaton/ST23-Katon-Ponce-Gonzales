
#include <Wire.h>     
#include <Adafruit_GFX.h>   
#include <Adafruit_SSD1306.h>  

#define AN 128     
#define ALT 64      

#define OLED_RES 4      
Adafruit_SSD1306 oled(AN, ALT, &Wire, OLED_RES);  


void setup() {
  Wire.begin();        
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
}

void loop() {
  oled.clearDisplay();    
  oled.setTextColor(WHITE);   
  oled.setCursor(0, 0);     
  oled.setTextSize(3);      
  oled.print("hola mundo");  
  oled.display();     
}
