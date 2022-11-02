//
//    FILE: DS28CM00.cpp
//  AUTHOR: Rob Tillaart
// PURPOSE: Library for the DS28CM00 unique identification chip.
// VERSION: 0.2.5
//     URL: https://github.com/RobTillaart/DS28CM00
//
// HISTORY: see changelog.md


#include "DS28CM00.h"


#define DS28CM00_DEVICEADDRESS      0x50
#define DS28CM00_UIDREGISTER        0x00
#define DS28CM00_CONTROLREGISTER    0x08


DS28CM00::DS28CM00(TwoWire *wire)
{
  _wire = wire;
}


#if defined(ESP8266) || defined(ESP32)
DS28CM00::DS28CM00(const uint8_t dataPin, const uint8_t clockPin)
{
  _wire = &Wire;
  if ((dataPin < 255) && (clockPin < 255))
  {
    _wire->begin(dataPin, clockPin);
  } else {
    _wire->begin();
  }
}
#endif


bool DS28CM00::begin()
{
  _wire->begin();
  return setI2CMode();
}


bool DS28CM00::isConnected()
{
  _wire->beginTransmission(DS28CM00_DEVICEADDRESS);
  int rv = _wire->endTransmission();
  return rv == 0;
}


bool DS28CM00::setMode(uint8_t mode)
{
  _wire->beginTransmission(DS28CM00_DEVICEADDRESS);
  _wire->write(DS28CM00_CONTROLREGISTER);
  _wire->write(mode);
  int rv = _wire->endTransmission();
  return rv == 0;
}


bool DS28CM00::getMode(uint8_t &mode)
{
  mode = DS28CM00_MODE_UNKNOWN;
  _wire->beginTransmission(DS28CM00_DEVICEADDRESS);
  _wire->write(DS28CM00_CONTROLREGISTER);
  int rv = _wire->endTransmission();
  if (rv != 0) return false;

  uint8_t read = _wire->requestFrom(DS28CM00_DEVICEADDRESS, 1);
  if (read < 1) return false;

  mode = _wire->read();
  return true;
}


bool DS28CM00::getUID(uint8_t *buffer)
{
  _wire->beginTransmission(DS28CM00_DEVICEADDRESS);
  _wire->write(DS28CM00_UIDREGISTER);
  int rv = _wire->endTransmission();
  if (rv != 0) return false;

  uint8_t read = _wire->requestFrom(DS28CM00_DEVICEADDRESS, 8);
  if (read < 8) return false;

  for (uint8_t i = 0; i < 8; i++) buffer[i] = _wire->read();

  return true;
}


// -- END OF FILE --

