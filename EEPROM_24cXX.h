#ifndef EEPROM_24cXX_h
#define EEPROM_24cXX_h
#include <Arduino.h>

#define C01 1
#define C02 2
#define C04 4
#define C08 8
#define C16 16
#define C32 32
#define C64 64
#define C128 128
#define C256 256
#define C512 512

class EEPROM_24cXX
{
public:
    EEPROM_24cXX(uint8_t address,  uint16_t type);
    void WriteByte(int address, byte value);
    uint16_t WriteInt(int address, int value);
    byte ReadByte(int address);
    uint16_t ReadInt(int address);
    uint32_t ByteSize();
    void begin();
private:
    byte chip_address;
    uint16_t chip_type;
};
#endif