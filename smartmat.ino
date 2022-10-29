#include <HX711.h>
#include "WiFi.h"
#include "ThingSpeak.h"


 

#define calibration_factor 420.0 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define WIFI_SSID  "Wokwi-GUEST"
#define WIFI_PASSWORD ""
unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "HFMXVATMNSEEB566";
WiFiClient  client;
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;
#define LOADCELL_DOUT_PIN  2
#define LOADCELL_SCK_PIN  4

HX711 scale;

void setup() {
  Serial.begin(115200);  //Initialize serial
   WiFi.begin( WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi

  Serial.print("Connecting to ");

  Serial.print(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {

    Serial.print(".");

    delay(500);

  }

  Serial.println();

  Serial.print("Connected");
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); 
}

void loop() {
  
  
  int weight=scale.get_units();
  
  if(weight>=5.0)
   { 
      ThingSpeak.writeField(myChannelNumber, 1, 1, myWriteAPIKey);
     Serial.print("Some one is on mat");
    
   }
  else
  {
    Serial.print("No one is on mat");
  }
  Serial.println();
}
