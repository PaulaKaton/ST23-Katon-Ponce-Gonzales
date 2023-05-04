//SI INTERNET//

#include <Wire.h>    
#include <Adafruit_GFX.h>   
#include <Adafruit_SSD1306.h>   
#include <ESP32Time.h>
#include "time.h"
#include <WiFi.h>

#define AN 128    
#define ALT 64       

#define OLED_RESET 4     
Adafruit_SSD1306 oled(AN, ALT, &Wire, OLED_RESET);  
#include "DHT.h"

#define DHTPIN 23     

#define DHTTYPE DHT11   

#define RELAY   18

#define BUTTON_HOURS  35
#define BOTON_MINUTES  34

//**********************************************************//
//* DECLARO DE FUNCION                                        **//
//**********************************************************//
void ASK_TIME(void);
void setup_rtc_ntp(void);

struct tm timeinfo;
ESP32Time rtc;


/// time
long unsigned int timestamp; 
const char *ntpServer = "south-america.pool.ntp.org";
const long gmtOffset_sec = -10800;
const int daylightOffset_sec = 0;

const char* ssid = "ORT-IoT";
const char* password = "OrtIOTnew22$2";

DHT dht(DHTPIN, DHTTYPE);

float t;
int hora[4] = {
  0,
  0,
  0,
  0
};
int primerHora;

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
  pinMode(BOTON_MINUTES, INPUT);
  Serial.begin(115200);
  Serial.println("Connecting to Wi-Fi...");
  initWiFi();
  setup_rtc_ntp();



}

void loop() {
  ASK_TIME();
  
}

void initWiFi() {
  WiFi.begin(ssid , password );
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}


//**********************************************************//
//* SETUP  rtc                                           **//
//**********************************************************//
void setup_rtc_ntp(void)
{
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  timestamp = time(NULL);
  rtc.setTime(timestamp + gmtOffset_sec);
}

//**********************************************************//
//* PIDE LA HORA AL SERVER O AL ESP32                     **//
//**********************************************************//
void ASK_TIME(void)
{
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("veo la hora del rtc interno ");
    timestamp = rtc.getEpoch() - gmtOffset_sec;
    timeinfo = rtc.getTimeStruct();
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  }
  else
  {
    Serial.print("NTP Time:");
    timestamp = time(NULL);
    
    hora[3] = timeinfo.tm_min;
    
    
    

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
    pantalla1();
  }

  return;
}