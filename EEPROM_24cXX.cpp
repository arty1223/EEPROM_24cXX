// #include <Arduino.h>
#include "EEPROM_24cXX.h"
#include <Wire.h>

EEPROM_24cXX::EEPROM_24cXX(uint8_t address, uint16_t type)
{
    chip_address = address;
    chip_type = type;  
}

void EEPROM_24cXX::begin(){  
  Wire.begin();  
}

void EEPROM_24cXX::WriteByte(int address, byte value){  
  Wire.beginTransmission(chip_address);
  if(chip_type > 2){
    Wire.write((int)(address >> 8));   
    Wire.write((int)(address & 0xFF)); 
  }else{
    Wire.write((int)(address));
  }   
  Wire.write(value);
  Wire.endTransmission();
  delay(2);  
}

byte EEPROM_24cXX::ReadByte(int address){
  Wire.beginTransmission(chip_address);  
  if(chip_type > 2){
    Wire.write((int)(address >> 8));   
    Wire.write((int)(address & 0xFF)); 
  }else{
    Wire.write((int)(address));
  } 
  Wire.endTransmission();  
  Wire.requestFrom(chip_address, 1);
  return(Wire.read());
}

uint16_t EEPROM_24cXX::WriteInt(int address, int value){
  WriteByte(address, value >> 8);
  WriteByte(address + 1, value & 0xff);
  return(address + 1);
}

uint16_t EEPROM_24cXX::ReadInt(int address){
  return((ReadByte(address) << 8) + ReadByte(address + 1));
}

uint32_t EEPROM_24cXX::ByteSize(){
  return(chip_type * 128);
}