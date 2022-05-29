#include "WiFi.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


const char* ssid = "Galaxy S20 FEF378";
const char* password = "111222333";
unsigned long previousMillis = 0;
unsigned long interval = 10000;
#define trigPin 5                                                                // ESP32 pin GIOP5 connected to ultrasnoic sensor's Trigger pin
#define echoPin 18           

int Sensor_value;      

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME  "hassan_abdullah" // Your Adafruit IO Username
#define MQTT_PASS "aio_SbVm17OCnbupZLPcjks5QgYR7ole" //  Your Adafruit IO AIO key
#define SOUND_SPEED 0.034                                                              //Define sound speed in cm/uS
#define CM_TO_INCH 0.393701 
long duration;                                                                          //Variable Definiatoins
float distanceCm;
float distanceInch;

    
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);
Adafruit_MQTT_Publish Sensor_data = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/WaterLevel");




void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

  void setup() {
  Serial.begin(115200);
  initWiFi();
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());
  pinMode(trigPin, OUTPUT);                                                        // Sets the Trigger Pin as an Output
  pinMode(echoPin, INPUT);                                                        // Sets the Echo Pin as an Input
}

void loop() {


  
  digitalWrite(trigPin, LOW);                                                       // Clear the trigger PIN
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);                                                      // Sets the trigPin on HIGH state for 10 micro seconds
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);                                                       // Sets the trigPin on LOW
  duration = pulseIn(echoPin, HIGH);                                               // Reads the echoPin, returns the sound wave travel time in microseconds

  distanceCm = duration * SOUND_SPEED / 2;                                        // Calculate the distance
  distanceInch = distanceCm * CM_TO_INCH;                                         // Convert to inches
  
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);                                      // Convert to inches

  MQTT_connect();

  Serial.print("value is  = ");
  Serial.println(distanceCm);
    if (! Sensor_data.publish(distanceCm))     {                     
         delay(2000);   
          }
          
 delay(2000);
}

void MQTT_connect() {

  int8_t again;

  if (mqtt.connected()) {

    return;

  }

  Serial.print("Connecting to Adafruit IO");

  uint8_t retry = 5;

  while ((again = mqtt.connect()) != 0) { 

       Serial.println(mqtt.connectErrorString(again));

       Serial.println("Retrying Adafruit connection in 5 seconds...");

       mqtt.disconnect();

       delay(5000);  

       retry--;

       if (retry == 0) {

         while (1);

       }

  }
  Serial.println("");

  Serial.println("Adafruit IO is Connected!");

}
