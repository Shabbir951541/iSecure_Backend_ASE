#include <HX711.h>
#include "FirebaseESP32.h"
#include "WiFi.h"
#include "HTTPClient.h"
#include <ESP32Servo.h>

Servo myservo;
int pos = 0; //variable to store server position
static const int servoPin = 5;
int count=0;
int motion=0;
int msensor=33;
int val=0;
const String url = "http://maker.ifttt.com/trigger/Eventdoor/json/with/key/hc7OZEZqmcfrgzqZ68412pBI9C9uOICI7d5n0v8uyOy";
const String url1 = "http://maker.ifttt.com/trigger/motion_detected/json/with/key/hc7OZEZqmcfrgzqZ68412pBI9C9uOICI7d5n0v8uyOy";
#define calibration_factor 420.0 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define WIFI_SSID  "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define FIREBASE_HOST "ase-project17-default-rtdb.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "niTp9IvVMJh8ohCc3Luoi0q7ZJ1uOANpR8h1YlRa"
FirebaseData fbdo;
FirebaseData pdata;
FirebaseData doordata;
FirebaseJson content;
#define LOADCELL_DOUT_PIN  2
#define LOADCELL_SCK_PIN  4
int buzzer=13;
int ledPin = 12;
HX711 scale;
HTTPClient http;

void setup() {
  Serial.begin(115200);  //Initialize serial
   WiFi.begin( WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("......");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); 
  Serial.print("Connected");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare();
  myservo.setPeriodHertz(50); //standard 50hz servo
  myservo.attach(servoPin, 10, 2800); 
  pinMode(buzzer, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(msensor, OUTPUT);
  
}

void loop() {
  
  
  int weight=scale.get_units();
  if(weight>=5.0)
   { 
     count++;
     Serial.println("Some one is on mat");
     String s="Some one is on mat";
     Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
     Firebase.setString(fbdo, "/User/zUacKz8K1RdesudN0H9bFXaeQxd2/Devices/smartmat", s);
     if(count==1)
     {
       http.begin(url);
       int httpResponseCode = http.GET();
     }
     if(Firebase.getString(pdata, "/User/zUacKz8K1RdesudN0H9bFXaeQxd2/Devices/person_status"))
     {  
        if(pdata.stringData()== "1")
         {  String s2="open";
            String s3="on";
            for (pos = 0; pos<=180; pos+=1)
              myservo.write(pos);
            Firebase.setString(fbdo, "/User/zUacKz8K1RdesudN0H9bFXaeQxd2/Devices/door", s2);
         }
         else if(pdata.stringData()=="0")
         {
           String s3="on";
           String s6="off";
           int i;
           val=digitalRead(msensor);
           if(val==HIGH)
           {
             motion++;
             //val=digitalRead(msensor);
             digitalWrite(ledPin, HIGH);
              tone(buzzer, 31); 
              Firebase.setString(fbdo, "/User/zUacKz8K1RdesudN0H9bFXaeQxd2/Devices/alarm", s3);
             if(motion==1)
              {
                http.begin(url1);
                 int httpResponseCode1 = http.GET();

             }
           }
           else{
                   digitalWrite(ledPin, LOW);
                 for(i=0;i<10;i++){
                  tone(buzzer,31);
                  Firebase.setString(fbdo, "/User/zUacKz8K1RdesudN0H9bFXaeQxd2/Devices/alarm", s3);
                 }
                noTone(buzzer);
                Firebase.setString(fbdo, "/User/zUacKz8K1RdesudN0H9bFXaeQxd2/Devices/alarm", s6);
           }
          

         }
       
     }
    
   }
  else
  {
    Serial.print("No one is on mat");
     String s1="No one is on mat";
     String s4="closed";
     String s5="off";
     Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
     Firebase.setString(fbdo, "/User/zUacKz8K1RdesudN0H9bFXaeQxd2/Devices/smartmat", s1);
     Firebase.setString(fbdo, "/User/zUacKz8K1RdesudN0H9bFXaeQxd2/Devices/door", s4);
     Firebase.setString(fbdo, "/User/zUacKz8K1RdesudN0H9bFXaeQxd2/Devices/alarm", s5);
     noTone(buzzer);
     digitalWrite(ledPin, LOW);
 }
  Serial.println();
}
