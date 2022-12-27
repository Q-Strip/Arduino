//  created   April 2021
//  by Daan van der Werf - Q-Strip
//  modified jan - 2022 
//  This example code is in the public domain.

// Date and time functions using a PCF8523 RTC connected via I2C and Wire lib
#include "RTClib.h"
#include <SD.h>
#include <SPI.h>
RTC_PCF8523 rtc;


File myFile;
int pinCS = 10; // Pin 10 on Arduino Uno

int sensorPin   = A0;
int sensorValue = 0;


void setup () {
  Serial.begin(57600);

   pinMode(pinCS, OUTPUT);
   
#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    //Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (! rtc.initialized() || rtc.lostPower()) {
  //  Serial.println("RTC is NOT initialized, let's set the time!");

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  }

// rtc.adjust(DateTime(2022, 12, 27, 8, 18, 0)); //Program Your current time here to synchronise RTC

  rtc.start();
  



 if (SD.begin())
  {
   // Serial.println("SD card is ready to use.");
  } else
  {
  //  Serial.println("SD card initialization failed");
    return;
  }
  
  // Create/Open file 
  myFile = SD.open("qstrip.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
    myFile.close(); // close the file
  }
}

void loop () {
    DateTime now = rtc.now();
    
//=>  if  ((now.hour() == int(21)) || (now.hour() == int(22)) || (now.hour() == int(23)) || (now.hour() == int(00)) || 
//    (now.hour() == int(1)) || (now.hour() == int(2)) || (now.hour() == int(3)) || (now.hour() == int(4)) || 
//    (now.hour() == int(5)) || (now.hour() == int(6)) || (now.hour() == int(7)) || (now.hour() == int(8))  )
//    {
      myFile = SD.open("qstrip.txt", FILE_WRITE); 
      if (myFile) {
        String Datum;
        char buf2[] = "YYYY/MM/DD-hh:mm:ss";
        Datum =now.toString(buf2);
        sensorValue = analogRead(sensorPin); 
        myFile.println(Datum+";"+sensorValue+";");
        myFile.close();
        
    //    Serial.println(Datum+";"+sensorValue+";");
        }
    delay(60000);
   // delay(10000);///development
    
//=>    }
    
//    else 
//    {
      //Serial.println("No job to run in this timeframe.. ");
//      delay(60000);
//      }
    
}
