#include "main.h"

u8 i2cdevReadByte(u8 dev, uint8_t devAddress, uint8_t memAddress, uint8_t *data);
u8 i2cdevReadBit(u8 dev, uint8_t devAddress, uint8_t memAddress, uint8_t bitNum, uint8_t *data);
u8 i2cdevWriteByte(u8 dev, uint8_t devAddress, uint8_t memAddress, uint8_t data);
u8 i2cdevWrite(u8 dev, uint8_t devAddress, uint8_t memAddress, uint16_t len, uint8_t *data);
u8 i2cdevRead(u8 dev, uint8_t devAddress, uint8_t memAddress, uint16_t len, uint8_t *data);
u8 i2cdevWriteBit(u8 dev, uint8_t devAddress, uint8_t memAddress, uint8_t bitNum, uint8_t data);
u8 i2cdevWriteBits(u8 dev, uint8_t devAddress, uint8_t memAddress, uint8_t bitStart, uint8_t length, uint8_t data);
void iicsearch(void);