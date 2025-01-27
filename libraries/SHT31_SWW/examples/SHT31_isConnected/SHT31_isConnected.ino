//
//    FILE: SHT31_isConnected.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo
//     URL: https://github.com/RobTillaart/SHT31_SWW


#include "SHT31_SWW.h"

#define SHT31_ADDRESS   0x44


SoftwareWire sw(6, 7);

uint32_t start;
uint32_t stop;
uint32_t connectionFails = 0;

SHT31_SWW sht;


void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("SHT31_SWW_LIB_VERSION: \t");
  Serial.println(SHT31_SWW_LIB_VERSION);

  sw.begin();
  sht.begin(SHT31_ADDRESS, &sw);
  sw.setClock(100000);

  uint16_t stat = sht.readStatus();
  Serial.print(stat, HEX);
  Serial.println();
}


void loop()
{
  if ( sht.isConnected()  )
  {
    start = micros();
    bool b = sht.read();         //  default = true/fast       slow = false
    stop = micros();

    int error = sht.getError();
    uint16_t stat = sht.readStatus();

    Serial.print(millis());
    Serial.print("\t");
    Serial.print(stop - start);
    Serial.print("\t");
    Serial.print(b, HEX);
    Serial.print("\t");
    Serial.print(error, HEX);
    Serial.print("\t");
    Serial.print(stat, HEX);
    Serial.print("\t");
    Serial.print(sht.getTemperature(), 1);
    Serial.print("\t");
    Serial.print(sht.getHumidity(), 1);
  }
  else
  {
    connectionFails++;
    Serial.print(millis());
    Serial.print("\tNot connected:\t");
    Serial.print(connectionFails);
    //  sht.reset();
  }
  Serial.println();
  delay(1000);
}


//  -- END OF FILE --

