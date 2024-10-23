// #include <Arduino.h>
#include "EEPROM_24cXX.h"
#include <Wire.h>

EEPROM_24cXX::EEPROM_24cXX(uint8_t address, int type)
{
  chip_address = address;
  chip_type = type;
}

void EEPROM_24cXX::begin()
{
  Wire.begin();
}

void EEPROM_24cXX::WriteByte(uint16_t address, byte value)
{
  Wire.beginTransmission(chip_address);
  if (chip_type > 2)
  {
    Wire.write((int)(address >> 8));
    Wire.write((int)(address & 0xFF));
  }
  else
  {
    Wire.write((int)(address));
  }
  Wire.write(value);
  Wire.endTransmission();
  delay(2);
}

byte EEPROM_24cXX::ReadByte(uint16_t address)
{
  Wire.beginTransmission(chip_address);
  if (chip_type > 2)
  {
    Wire.write((int)(address >> 8));
    Wire.write((int)(address & 0xFF));
  }
  else
  {
    Wire.write((int)(address));
  }
  Wire.endTransmission();
  Wire.requestFrom(chip_address, 1);
  return (Wire.read());
}

uint16_t EEPROM_24cXX::WriteInt(uint16_t address, int value)
{
  WriteByte(address, value >> 8);
  WriteByte(address + 1, value & 0xff);
  return (address + 1);
}

uint16_t EEPROM_24cXX::WriteFloat(uint16_t address, float value)
{
  union datatype
  {
    float f;
    byte b[4];
  };
  datatype t;
  t.f = value;
  for (int i = 0, address; i < 4; i++, address++)
  {
    WriteByte(address, t.b[i]);
  }
  return (address + 3);
}

float EEPROM_24cXX::ReadFloat(uint16_t address){
  union datatype
  {
    float f;
    byte b[4];
  };
  datatype t;  
  for (int i = 0, address; i < 4; i++, address++)
  {
    t.b[i] = ReadByte(address);
  }
  return t.f;

}
uint16_t EEPROM_24cXX::ReadInt(uint16_t address)
{
  return ((ReadByte(address) << 8) + ReadByte(address + 1));
}

uint32_t EEPROM_24cXX::ByteSize()
{
  return (chip_type * 128);
}