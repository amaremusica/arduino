#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>
#include <SuplaDevice.h>

/*
 * SUPLA DEVICE - ARDUINO - ETHERNET SHIELD & DHT SENSOR
 * Author: Przemyslaw Zygmunt <przemek@supla.org>
 *
 * This example requires DHT sensor library installed. 
 * https://github.com/adafruit/DHT-sensor-library
 */

#define DHTPIN 24
#define DHTTYPE DHT22
 
// Setup a DHT instance
DHT dht(DHTPIN, DHTTYPE);


// DS18B20 Sensor read implementation
void get_temperature_and_humidity(int channelNumber, double *temp, double *humidity) {

    *temp = dht.readTemperature();
    *humidity = dht.readHumidity();

    if ( isnan(*temp) || isnan(*humidity) ) {
      *temp = -275;
      *humidity = -1;
    }

}

void setup() {

  Serial.begin(9600);

  // Init DHT library 
  dht.begin(); 
  
  // Set temperature/humidity callback
  SuplaDevice.setTemperatureHumidityCallback(&get_temperature_and_humidity);

  // ﻿Replace the falowing GUID
  char GUID[SUPLA_GUID_SIZE] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  // ﻿with GUID that you can retrieve from https://www.supla.org/arduino/get-guid


  // Ethernet MAC address
  uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};

  /*
   * Having your device already registered at cloud.supla.org,
   * you want to change CHANNEL sequence or remove any of them,
   * then you must also remove the device itself from cloud.supla.org.
   * Otherwise you will get "Channel conflict!" error.
   */
    
  // CHANNEL0 - RELAY
  SuplaDevice.addRelay(44, true);           // ﻿44 - ﻿Pin number where the relay is connected      
                                      // Call SuplaDevice.addRelay(44, true) with an extra "true" parameter 
                                      // to enable "port value inversion"
                                      // where HIGH == LOW, and LOW == HIGH   

  // CHANNEL1 - RELAY
  SuplaDevice.addRelay(45, true);           // 45 - ﻿﻿Pin number where the relay is connected   

  // CHANNEL3 - TWO RELAYS (Roller shutter operation)
  SuplaDevice.addRollerShutterRelays(46,     // 46 - ﻿﻿Pin number where the 1st relay is connected   
                                     47, true);    // 47 - ﻿Pin number where the 2nd relay is connected  

  // CHANNEL4 - Opening sensor (Normal Open)
  SuplaDevice.addSensorNO(A0); // A0 - ﻿Pin number where the sensor is connected
                               // Call SuplaDevice.addSensorNO(A0, true) with an extra "true" parameter
                               // to enable the internal pull-up resistor


  // CHANNEL5 - Opening sensor (Normal Open)
  SuplaDevice.addSensorNO(A1); // A1 - ﻿Pin number where the sensor is connected


  // CHANNEL6 - DHT22 Sensor
  // SuplaDevice.addDHT11();
  // SuplaDevice.addAM2302();
  SuplaDevice.addDHT22();


  /*
   * SuplaDevice Initialization.
   * Server address, LocationID and LocationPassword are available at https://cloud.supla.org 
   * If you do not have an account, you can create it at https://cloud.supla.org/account/create
   * SUPLA and SUPLA CLOUD are free of charge
   * 
   */

  SuplaDevice.begin(GUID,              // Global Unique Identifier 
                    mac,               // Ethernet MAC address
                    "svr1.supla.org",  // SUPLA server address
                    0,                 // Location ID 
                    "");               // Location Password
    
}

void loop() {
  SuplaDevice.iterate();
}
