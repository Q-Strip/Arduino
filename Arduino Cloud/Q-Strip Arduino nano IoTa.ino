#include "arduino_secrets.h"
/* 
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/cf529125-d989-4398-b98c-a8132d61fce5 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  int sweat;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"
int sensorPin   = A0;

void setup() {
  Serial.begin(9600);   // Initialize serial and wait for port to open:
  delay(1500);          // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  initProperties();     // Defined in thingProperties.h

  ArduinoCloud.begin(ArduinoIoTPreferredConnection);   // Connect to Arduino IoT Cloud
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}



void loop() {
  ArduinoCloud.update();
  // Your code here
  sweat = analogRead(sensorPin);
  Serial.println(sweat);
   
  ArduinoCloud.addProperty(sweat, READWRITE, ON_CHANGE, onSweatChange); // linking the read & write variable with its function and updating it on change
 delay(1000); 
}


void onVariableName01Change() {
  // Do something
    sweat = analogRead(sensorPin);
}
/*
  Since Sweat is READ_WRITE variable, onSweatChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onSweatChange()  {
  // Add your code here to act upon Sweat change
    sweat = analogRead(sensorPin);
}


