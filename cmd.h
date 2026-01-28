#ifndef __CMD_H__
#define __CMD_H__

#include "main.h"

typedef enum
{
  CMD_WD_UNDEF         = 0x00,
  CMD_SET_WD_INTERVAL  = 0x01,
  CMD_GET_WD_INTERVAL  = 0x02,
  CMD_SET_SLEEP_TIME   = 0x03,
  CMD_GET_SLEEP_TIME   = 0x04,
  CMD_CLEAR_WATCHDOG   = 0x05,
  CMD_SWITCH_OFF       = 0x06,
  CMD_SET_EEPROM_INDEX = 0x07,
  CMD_EEPROM_LOAD      = 0x08,
  CMD_EEPROM_SAVE      = 0x09,
  CMD_POWER_OFF_0      = 0x0A,
  CMD_POWER_OFF_1      = 0x0B,
  CMD_EXT_RESET        = 0x0C,
  CMD_EEPROM_READ      = 0x0D,
  CMD_EEPROM_WRITE     = 0x0E,
  CMD_GET_RESTARTS     = 0x0F,
  CMD_NBR_OF           = 16
 } cmd_et;


typedef struct
{
  uint8_t tx_bytes;
  uint8_t rx_bytes;
  uint8_t *data;
} cmd_data_st;

uint8_t cmd_get_rx_len(uint8_t cmd);

uint8_t cmd_get_tx_len(uint8_t cmd);

uint8_t *xxcmd_get_data_ptr(uint8_t cmd);

void cmd_execute_cmd(uint8_t cmd);

void cmd_get_data(uint8_t cmd);

#endif