/*
//  created   April 2021
//  by Daan van der Werf - Q-Strip
//  modified jan - 2022 
//  This example code is in the public domain.

*/

#include <ArduinoBLE.h>


//----------------------------------------------------------------------------------------------------------------------
// BLE UUIDs
//----------------------------------------------------------------------------------------------------------------------

// https://www.bluetooth.com/specifications/assigned-numbers/environmental-sensing-service-characteristics/

//#define BLE_UUID_ENVIRONMENTAL_SENSING_SERVICE    "181A"
//#define BLE_UUID_TEMPERATURE                      "2A6E" 
//#define BLE_UUID_HUMIDITY                         "2A6F"

#define BLE_UUID_ENVIRONMENTAL_SENSING_SERVICE    "{00002A38-0000-1000-8000-00805F9B34FB}"
#define BLE_UUID_TEMPERATURE                      "{00002A37-0000-1000-8000-00805F9B34FB}" 
#define BLE_UUID_HUMIDITY                         "{00002A39-0000-1000-8000-00805F9B34FB}"


//----------------------------------------------------------------------------------------------------------------------
// BLE
//----------------------------------------------------------------------------------------------------------------------

BLEService environmentalSensingService( BLE_UUID_ENVIRONMENTAL_SENSING_SERVICE );
BLEShortCharacteristic temperatureCharacteristic( BLE_UUID_TEMPERATURE, BLERead | BLENotify );
BLEUnsignedShortCharacteristic humidityCharacteristic( BLE_UUID_HUMIDITY, BLERead | BLENotify );

const int BLE_LED_PIN = LED_BUILTIN;
//const int RSSI_LED_PIN = LED_PWR;
const int LINK_LED_PIN = 2   ;
int sensorPin = A0;
void setup()
{
  Serial.begin( 9600 );
  // while ( !Serial );

  pinMode( BLE_LED_PIN, OUTPUT );
//  pinMode( RSSI_LED_PIN, OUTPUT );
  pinMode( LINK_LED_PIN, OUTPUT );
  
  // Without Serial when using USB power bank HTS sensor seems to needs some time for setup
  delay( 10 );

  if( setupBleMode() )
  {
  //  digitalWrite( BLE_LED_PIN, HIGH );
    
  }
} // setup


void loop()
{
  static long previousMillis = 0;

  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  if ( central )
  {
    Serial.print( "Connected to central: " );
    Serial.println( central.address() );
    digitalWrite( LINK_LED_PIN, HIGH );

    while ( central.connected() )
    {
      long interval = 1000;
      unsigned long currentMillis = millis();
      if( currentMillis - previousMillis > interval )
      {
        previousMillis = currentMillis;

        Serial.print( "Central RSSI: " );
        Serial.println( central.rssi() );
        Serial.print("Sweat Value:");
        Serial.println(analogRead(sensorPin));
        
        if( central.rssi() != 0 )
        {
//          digitalWrite( RSSI_LED_PIN, LOW );

          // BLE defines Temperature UUID 2A6E Type sint16
          // Unit is in degrees Celsius with a resolution of 0.01 degrees Celsius
          uint16_t temperature = analogRead(sensorPin);//round( HTS.readTemperature() * 100.0 );
          temperatureCharacteristic.writeValue( temperature );

          // BLE defines Humidity UUID 2A6F Type uint16
          // Unit is in percent with a resolution of 0.01 percent
          uint16_t humidity = 0;//round( HTS.readHumidity() * 100.0 );
          humidityCharacteristic.writeValue( humidity );
        }
        else
        {
//          digitalWrite( RSSI_LED_PIN, HIGH );
          
        }
        
      } // intervall
    } // while connected
    digitalWrite( LINK_LED_PIN, LOW );
    Serial.print( F( "Disconnected from central: " ) );
    Serial.println( central.address() );
  } // if central
} // loop



bool setupBleMode()
{
  if ( !BLE.begin() )
  {
    return false;
  }

  // set advertised local name and service UUID:
  BLE.setDeviceName( "Q-Strip Device" );
  BLE.setLocalName( "Q-Strip Device" );
  BLE.setAdvertisedService( environmentalSensingService );

  // BLE add characteristics
  environmentalSensingService.addCharacteristic( temperatureCharacteristic );
  environmentalSensingService.addCharacteristic( humidityCharacteristic );

  // add service
  BLE.addService( environmentalSensingService );

  // set the initial value for the characeristic:
  temperatureCharacteristic.writeValue( 0 );
  humidityCharacteristic.writeValue( 0 );

  // start advertising
  BLE.advertise();

  return true;
}
