//
//    FILE: SHT31_heater.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo heater functions
//     URL: https://github.com/RobTillaart/SHT31_SWW


#include "SHT31_SWW.h"

#define SHT31_ADDRESS   0x44


SoftwareWire sw(6, 7);

uint32_t start;
uint32_t stop;

SHT31_SWW sht;
uint16_t status;


void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("SHT31_SWW_LIB_VERSION: \t");
  Serial.println(SHT31_SWW_LIB_VERSION);

  sw.begin();
  sht.begin(SHT31_ADDRESS, &sw);
  sw.setClock(100000);

  sht.setHeatTimeout(30);  // heater timeout 30 seconds, just for demo.

  status = sht.readStatus();
  printHeaterStatus(status);

  sht.heatOn();

  while (sht.isHeaterOn())
  {
    status = sht.readStatus();
    printHeaterStatus(status);
    sht.read();
    Serial.println(sht.getTemperature());
    delay(10000);
  }

  Serial.println("switched off");
}


void loop()
{
  // forced switch off
  if (status & SHT31_STATUS_HEATER_ON) sht.heatOff();
}


void printHeaterStatus(uint16_t status)
{
  Serial.print(millis());
  Serial.print("\tHEATER: ");
  if (status & SHT31_STATUS_HEATER_ON)
  {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }
}


// -- END OF FILE --
