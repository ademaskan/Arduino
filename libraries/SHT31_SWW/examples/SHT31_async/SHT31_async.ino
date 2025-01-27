//
//    FILE: SHT31_async.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo async interface
//     URL: https://github.com/RobTillaart/SHT31_SWW


#include "SHT31_SWW.h"

#define SHT31_ADDRESS   0x44


SoftwareWire sw(6, 7);

uint32_t start;
uint32_t stop;
uint32_t cnt;

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
  
  sht.requestData();
  cnt = 0;
}


void loop()
{
  if (sht.dataReady())
  {
    start = micros();
    bool success  = sht.readData();   // default = true = fast
    stop = micros();
    sht.requestData();                // request for next sample

    Serial.print("\t");
    Serial.print(stop - start);
    Serial.print("\t");
    if (success == false)
    {
      Serial.println("Failed read");
    }
    else
    {
      Serial.print(sht.getTemperature(), 1);
      Serial.print("\t");
      Serial.print(sht.getHumidity(), 1);
      Serial.print("\t");
      Serial.println(cnt);
      cnt = 0;
    }
  }
  cnt++; // simulate other activity
}


// -- END OF FILE --
