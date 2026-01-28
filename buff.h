#ifndef __BUFF_H__
#define __BUFF_H__

#include "main.h"

uint32_t buff_get_u32(uint8_t *buff, uint8_t indx);

uint16_t buff_get_u16(uint8_t *buff, uint8_t indx);

void buff_set_u32(uint8_t *buff, uint8_t indx, uint32_t u32);

void buff_set_u16(uint8_t *buff, uint8_t indx, uint16_t u16);

#endif