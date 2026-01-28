#include "Arduino.h"
#include "main.h"

uint32_t buff_get_u32(uint8_t *buff, uint8_t indx)
{
  uint32_t u32 = 0;
  u32 |= (uint32_t)buff[indx++] << 24;
  u32 |= (uint32_t)buff[indx++] << 16;
  u32 |= (uint32_t)buff[indx++] << 8;
  u32 |= (uint32_t)buff[indx] << 0;
  return u32;
}

uint16_t buff_get_u16(uint8_t *buff, uint8_t indx)
{
  uint32_t u16 = 0;
  u16 |= (uint16_t)buff[indx++] << 8;
  u16 |= (uint16_t)buff[indx] << 0;
  return u16;
}

void buff_set_u32(uint8_t *buff, uint8_t indx, uint32_t u32)
{
  buff[indx++] = (uint8_t)((u32 >> 24) & 0x000000FF);
  buff[indx++] = (uint8_t)((u32 >> 16) & 0x000000FF);
  buff[indx++] = (uint8_t)((u32 >> 8) & 0x000000FF);
  buff[indx] =   (uint8_t)((u32 >> 0) & 0x000000FF);
}

void buff_set_u16(uint8_t *buff, uint8_t indx, uint16_t u16)
{
  buff[indx++] = (uint8_t)((u16 >> 8) & 0x000000FF);
  buff[indx] =   (uint8_t)((u16 >> 0) & 0x000000FF);
}

