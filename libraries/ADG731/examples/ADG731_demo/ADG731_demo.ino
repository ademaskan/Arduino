//
//    FILE: ADG731_demo.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: minimal sketch
//     URL: https://github.com/RobTillaart/ADG731


#include "ADG731.h"


ADG731 ADG(10, 11, 12);

uint32_t start, stop;

void setup()
{
  Serial.begin(115200);
  Serial.print("ADG731_LIB_VERSION: ");
  Serial.println(ADG731_LIB_VERSION);
  delay(100);
  
  start = micros();
  ADG.setChannel(0);
  stop = micros();
  Serial.print("Time:\t");
  Serial.println(stop - start);
  delay(100);

  start = micros();
  ADG.setChannel(1);
  stop = micros();
  Serial.print("Time:\t");
  Serial.println(stop - start);
  delay(100);
}


void loop()
{
  for (int ch = 0; ch < 32; ch++)
  {
    ADG.setChannel(ch);

    Serial.print(ch);
    Serial.print("\t");
    Serial.println(ADG.getChannel());
    delay(1000);
  }
}


//  -- END OF FILE --
