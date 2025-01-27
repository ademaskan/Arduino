//
//    FILE: m5rotate8.cpp
//  AUTHOR: Rob Tillaart
// VERSION: 0.2.0
// PURPOSE: Arduino library for M5 8ROTATE 8x rotary encoders
//     URL: https://github.com/RobTillaart/M5ROTATE8


#include "m5rotate8.h"


#define M5ROTATE8_REG_ADDRESS            0xFF
#define M5ROTATE8_REG_VERSION            0xFE
#define M5ROTATE8_REG_BASE_ABS           0x00
#define M5ROTATE8_REG_BASE_REL           0x20
#define M5ROTATE8_REG_BASE_RESET         0x40
#define M5ROTATE8_REG_BASE_BUTTON        0x50
#define M5ROTATE8_REG_SWITCH             0x60
#define M5ROTATE8_REG_RGB                0x70


M5ROTATE8::M5ROTATE8(uint8_t address, TwoWire *wire)
{
  _address = address;
  _wire = wire;
}


#if defined (ESP8266) || defined(ESP32)
bool M5ROTATE8::begin(int dataPin, int clockPin)
{
  _wire = &Wire;
  if ((dataPin < 255) && (clockPin < 255))
  {
    _wire->begin(dataPin, clockPin);
  } else {
    _wire->begin();
  }
  if (! isConnected()) return false;
  return true;
}
#endif


bool M5ROTATE8::begin()
{
  _wire->begin();
  if (! isConnected()) return false;
  return true;
}


bool M5ROTATE8::isConnected()
{
  _wire->beginTransmission(_address);
  return (_wire->endTransmission() == 0);
}


bool M5ROTATE8::setAddress(uint8_t address)
{
  _address = address;
  write8(M5ROTATE8_REG_ADDRESS, _address);
  return isConnected();
}


uint8_t M5ROTATE8::getAddress()
{
  return _address;
}


uint8_t M5ROTATE8::getVersion()
{
  return read8(M5ROTATE8_REG_VERSION);
}


////////////////////////////////////////////////
//
//  ROTARY ENCODER PART
//
int32_t M5ROTATE8::getAbsCounter(uint8_t channel)
{
  return read32(M5ROTATE8_REG_BASE_ABS + (channel << 2));
}


bool M5ROTATE8::setAbsCounter(uint8_t channel, int32_t value)
{
  return write32(M5ROTATE8_REG_BASE_ABS + (channel << 2), value);
}


int32_t M5ROTATE8::getRelCounter(uint8_t channel)
{
  return read32(M5ROTATE8_REG_BASE_REL + (channel << 2));
}


bool M5ROTATE8::getKeyPressed(uint8_t channel)
{
  if (channel > 7)
  {
    return false;
  }
  return (0 == read8(M5ROTATE8_REG_BASE_BUTTON + channel));
}


bool M5ROTATE8::resetCounter(uint8_t channel)
{
  if (channel > 7)
  {
    return false;
  }
  write8(M5ROTATE8_REG_BASE_RESET + channel, 1);
  return true;
}


void M5ROTATE8::resetAll()
{
  for (int channel = 0; channel < 8; channel++)
  {
    write8(M5ROTATE8_REG_BASE_RESET + channel, 1);
  }
}


////////////////////////////////////////////////
//
//  INPUT SWITCH PART
//
uint8_t M5ROTATE8::inputSwitch()
{
  return read8(M5ROTATE8_REG_SWITCH);
}


////////////////////////////////////////////////
//
//  LED PART
//
bool M5ROTATE8::writeRGB(uint8_t channel, uint8_t R, uint8_t G, uint8_t B)
{
  if (channel > 8)
  {
    return false;
  }
  write24(M5ROTATE8_REG_RGB + (channel * 3), R, G, B);
  return true;
}


bool M5ROTATE8::setAll(uint8_t R, uint8_t G, uint8_t B)
{
  for (uint8_t ch = 0; ch < 9; ch++)
  {
    write24(M5ROTATE8_REG_RGB + (ch * 3), R, G, B);
  }
  return true;
}


bool M5ROTATE8::allOff()
{
  return setAll(0, 0, 0);
}


////////////////////////////////////////////////
//
//  PRIVATE
//
bool M5ROTATE8::write8(uint8_t reg, uint8_t value)
{
  _wire->beginTransmission(_address);
  _wire->write(reg);
  _wire->write(value);
  _error = _wire->endTransmission();
  return (_error == 0);
}


uint8_t M5ROTATE8::read8(uint8_t reg)
{
  _wire->beginTransmission(_address);
  _wire->write(reg);
  _error = _wire->endTransmission();
  if (_error != 0)
  {
    //  error handling
    return 0;
  }
  if (_wire->requestFrom(_address, (uint8_t)1) != 1)
  {
    //  error handling
    return 0;
  }
  return _wire->read();
}


bool M5ROTATE8::write24(uint8_t reg, uint8_t R, uint8_t G, uint8_t B)
{
  _wire->beginTransmission(_address);
  _wire->write(reg);
  _wire->write(R);
  _wire->write(G);
  _wire->write(B);
  _error = _wire->endTransmission();
  return (_error == 0);
}


bool M5ROTATE8::write32(uint8_t reg, uint32_t value)
{
  _wire->beginTransmission(_address);
  _wire->write(reg);
  _wire->write(value & 0xFF);
  value >>= 8;
  _wire->write(value & 0xFF);
  value >>= 8;
  _wire->write(value & 0xFF);
  value >>= 8;
  _wire->write(value);
  _error = _wire->endTransmission();
  return (_error == 0);
}


uint32_t M5ROTATE8::read32(uint8_t reg)
{
  _wire->beginTransmission(_address);
  _wire->write(reg);
  _error = _wire->endTransmission();
  if (_error != 0)
  {
    //  error handling
    return 0;
  }
  if (_wire->requestFrom(_address, (uint8_t)4) != 4)
  {
    //  error handling
    return 0;
  }
  uint32_t value = 0;
  value += (_wire->read());
  value += (((uint32_t)_wire->read()) << 8 );
  value += (((uint32_t)_wire->read()) << 16);
  value += (((uint32_t)_wire->read()) << 24);
  return value;
}



//  -- END OF FILE --
