#include <Wire.h>
#include <SeeedOLED.h>
const int buttonPin =7 ; 
//const int buttonPin1 = 7; 

int tempPin = 2; ///temp of nozzle heater
int buttonState = 1; 
int buttonState2 = 1;
int sensorPin = A0;
int sensorValue = 0;
int telling;
int maxheight;

const int masterPin = 2;     // the number of the pushbutton pin
const int ledPin =  3;      // the number of the LED pin
int masterbutton = 0;



String q;
char buffer[256];

void setup() {
    // declare the ledPin as an OUTPUT:
    //Serial.begin(9600);    
    pinMode(buttonPin, INPUT);
 //   pinMode(buttonPin1, INPUT);
    pinMode(8, OUTPUT); //relais pin fan 
    pinMode(4, OUTPUT); //relais pin heater
    pinMode(2, OUTPUT); //led pin   
  
    pinMode(ledPin, OUTPUT);
    pinMode(masterPin, INPUT_PULLUP);
   
    
    Wire.begin();
    SeeedOled.init();  //initialze SEEED OLED display
    Serial.begin(9600);
    SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
    SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
    SeeedOled.setPageMode();           //Set addressing mode to Page Mode  
    //SeeedOled.setTextXY(0,0);          //Set the cursor to Xth Page, Yth Column  
    //SeeedOled.putString("Starting Up.."); //Print the String 

    //SeeedOled.setTextXY(6,0);          //Set the cursor to Xth Page, Yth Column  
    //SeeedOled.putString("Q-Strip Tester"); //Print the String 

   
}
void loop() {

 masterbutton = digitalRead(masterPin);

Serial.println(masterbutton);
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (masterbutton == LOW) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }

   int reading = analogRead(A2);  
     // converting that reading to voltage, for 3.3v arduino use 3.3
     float voltage = reading * 5.0;
     voltage /= 1024.0; 
     
     // now print out the temperature
     float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree with 500 mV offset
                                                   //to degrees ((voltage - 500mV) times 100)
     Serial.print(temperatureC); Serial.println(" degrees C");


    buttonState  = digitalRead(buttonPin); 
   // buttonState2 = digitalRead(buttonPin1);

    if (buttonState == LOW) {
       maxheight = maxheight +50;
       if (maxheight > 700 ){
        maxheight = 0;
        }
    }
   
    else {
     
    }
    
    sensorValue = analogRead(sensorPin); // read the value from the sensor:
    
    if (sensorValue > maxheight){
      digitalWrite(8, HIGH);//relais aan fan

        if (temperatureC < 50) {////if heater aint too hot run..
          digitalWrite(4, HIGH);//relais aan heater
          } 

      if (temperatureC > 50) {////if heater is getting too hot run stop relair heating
          digitalWrite(4, LOW);//relais uit heater
          }    
     
      digitalWrite(6, HIGH);//led




      delay(60000);
      }else
     {
      digitalWrite(8, LOW);//relais uit fan
      digitalWrite(4, LOW);//relais uit heater
      digitalWrite(6, LOW);//led uit
      } 

      
      if (masterbutton == LOW){ //if the masterbutton is on to heat up bed
         
         digitalWrite(8, HIGH);//relais aan fan
        if (temperatureC < 50) {////if heater aint too hot run..
          digitalWrite(4, HIGH);//relais aan heater
           delay(10000); //keep 10 secs on
          } else {
            digitalWrite(4, LOW);//relais uit heater
            delay(10000); //keep 10 secs of
          }
        

      }
    
    sensorValue = analogRead(sensorPin); // read the value from the sensor:
    Serial.print("Q-Strip Sensor = " );          // Print the line in the terminal debugger
    Serial.println(sensorValue);         // Print the line in the terminal debugger
    
    q  = String(analogRead(sensorPin));
    
    SeeedOled.setTextXY(0,0);             //Set the cursor to Xth Page, Yth Column  
    SeeedOled.putString("Q-Strip Cooling"); //Print the String 

    SeeedOled.setTextXY(3,0);             //Set the cursor to Xth Page, Yth Column  
    SeeedOled.putString("Value    ");       //Print the String

    SeeedOled.setTextXY(3,8);          //Set the cursor to Xth Page, Yth Column  
    SeeedOled.putString("   "); //Print the String
    
    SeeedOled.setTextXY(3,8);          //Set the cursor to Xth Page, Yth Column  
    SeeedOled.putNumber(sensorValue);     //Print the String
//
    SeeedOled.setTextXY(4,0);             //Set the cursor to Xth Page, Yth Column  
    SeeedOled.putString("Trigger   ");       //Print the String

    SeeedOled.setTextXY(4,8);          //Set the cursor to Xth Page, Yth Column  
    SeeedOled.putString("        "); //Print the String
    
    SeeedOled.setTextXY(4,8);          //Set the cursor   to Xth Page, Yth Column  
    SeeedOled.putNumber(maxheight);     //Print the String

   SeeedOled.setTextXY(5,0);          //Set the cursor to Xth Page, Yth Column  
    SeeedOled.putString("Temp.    "); //Print the String

     SeeedOled.setTextXY(5,8);          //Set the cursor to Xth Page, Yth Column  
    SeeedOled.putString("         "); //Print the String
 
    SeeedOled.setTextXY(5,8);          //Set the cursor to Xth Page, Yth Column  
    SeeedOled.putFloat(temperatureC);     //Print the String

    SeeedOled.setTextXY(6,0);          //Set the cursor to Xth Page, Yth Column  
    SeeedOled.putString("  "); //Print the String 
    
    
    
    delay(50);
}
