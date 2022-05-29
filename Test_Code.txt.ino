
#define SOUND_SPEED 0.034                                                              //Define sound speed in cm/uS
#define CM_TO_INCH 0.393701                                                           //Convert CM to Inches
#define POWER_PIN  17                                                                // ESP32 pin GIOP17 connected to water sensor's VCC pin
#define SIGNAL_PIN 36                                                               // ESP32 pin GIOP36 (ADC0) connected to water sensor's signal pin
#define Relay 26                                                                   // ESP32 pin GIOP26 connected to Relay signal pin 2for pump 
#define LED 2                                                                     // ESP32 pin GIOP2 connected to internal LED of ESP32                                                                       
#define trigPin 5                                                                // ESP32 pin GIOP5 connected to ultrasnoic sensor's Trigger pin
#define echoPin 18                                                              //ESP32 pin GIOP18 connected to ultrasnoic sensor's Echo pin


long duration;                                                                          //Variable Definiatoins
int value = 0;
float distanceCm;
float distanceInch;
unsigned long previousMillis = 0;
unsigned long interval = 10000;
int MAX = 1000;                                                                       //Maximum Water Level
int MIN = 500;                                                                       //Minimum Water Level



void setup() {
  pinMode(trigPin, OUTPUT);                                                        // Sets the Trigger Pin as an Output
  pinMode(echoPin, INPUT);                                                        // Sets the Echo Pin as an Input
  pinMode(Relay, OUTPUT);                                                        // Set the Relay pin as output 
  pinMode(LED, OUTPUT);                                                         //Set the LED pin as output
  pinMode(POWER_PIN, OUTPUT);                                                  //Set the POWER PIN as OUTPUT
  digitalWrite(POWER_PIN, LOW);                                               //Clear the power pin, Set to Low (not Working)
}

void loop() {

   Waterlevel();                                                                    //Call Water Senor Reading Function
   UltraSensor();                                                                  //Call Ultra Sonic Senor Reading Function
  if (value < MIN) {                                                              //If water Level decreases the MIN level turn on the pump 
    digitalWrite(Relay, HIGH);
    digitalWrite(LED, HIGH);
  }
  else if ((distanceCm < 50.0)) {                                                //When ultrasonic sensor detects an object near it
    if(value <= MAX){                                                          //Turn on Pump if water level is less than MAX Level
    digitalWrite(Relay, HIGH);
    digitalWrite(LED, HIGH);
    }
    else                                                                        //If water is more then the MAX limit, pump will not work
    {
      digitalWrite(Relay, LOW);
      digitalWrite(LED, LOW);
    }
     
  }
   else                                                                         //If water level is more than MIN level or the no object detected, pump is off
  {
    digitalWrite(Relay, LOW);
    digitalWrite(LED, LOW);
  }
  delay(2000); 
}



void Waterlevel()                                                                         //Funtion to detect Water level
{
  digitalWrite(POWER_PIN, HIGH);                                                         // turn the sensor ON
  delay(20);                                                                            // wait 20 milliseconds
  value = analogRead(SIGNAL_PIN);                                                      // read the analog value from sensor
  digitalWrite(POWER_PIN, LOW);                                                       // turn the sensor OFF
  Serial.print("The water sensor value: ");                                          //Printing Values of sensor in serial moniter
  Serial.println(value);
}


    
void UltraSensor() {                                                                 //Funtion to detect Water level
  
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
  Serial.println(distanceInch);
}

