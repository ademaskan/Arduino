//
//    FILE: SHT31_lastRead.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo
//     URL: https://github.com/RobTillaart/SHT31_SWW


#include "SHT31_SWW.h"

#define SHT31_ADDRESS   0x44


SoftwareWire sw(6, 7);

uint32_t start;
uint32_t stop;

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
  sht.read();         // default = true/fast       slow = false
  Serial.print("\t");
  Serial.print(sht.lastRead());
  Serial.print("\t");
  Serial.print(sht.getTemperature(), 1);
  Serial.print("\t");
  Serial.println(sht.getHumidity(), 1);
  delay(100);
}


// -- END OF FILE --
