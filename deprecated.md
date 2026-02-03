#ifndef __BUFF_H__
#define __BUFF_H__

#include "main.h"

uint32_t buff_get_u32(uint8_t *buff, uint8_t indx);

uint16_t buff_get_u16(uint8_t *buff, uint8_t indx);

void buff_set_u32(uint8_t *buff, uint8_t indx, uint32_t u32);

void buff_set_u16(uint8_t *buff, uint8_t indx, uint16_t u16);

#endif

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

#include "Arduino.h"
#include <Wire.h>
#include "main.h"
#include "cmd.h"
#include "io.h"
#include "buff.h"
#include "eep.h"
#include "edog.h"


uint8_t dummy_byte;
uint8_t user_eeprom_indx = EEPROM_USER_0;
extern volatile main_data_st main_data;
extern volatile restarts_st restarts;
extern i2c_buff_st i2c_buff;

const cmd_data_st cmd_data[CMD_NBR_OF] = 
{ //                       TX RX  Ptr
  [CMD_WD_UNDEF]        = { 0, 0, &dummy_byte},
  [CMD_SET_WD_INTERVAL] = { 0, 4, (uint8_t *)&main_data.wd_interval_ms},
  [CMD_GET_WD_INTERVAL] = { 4, 0, (uint8_t *)&main_data.wd_interval_ms},
  [CMD_SET_SLEEP_TIME]  = { 0, 4,  (uint8_t *)&main_data.sleep_time_cycles},
  [CMD_GET_SLEEP_TIME]  = { 4, 0, &dummy_byte},
  [CMD_CLEAR_WATCHDOG]  = { 0, 0, &dummy_byte},
  [CMD_SWITCH_OFF]      = { 0, 0, &dummy_byte},
  [CMD_SET_EEPROM_INDEX] = { 1, 0, (uint8_t *)&restarts.internal},
  [CMD_EEPROM_LOAD]     = { 0, 0, &dummy_byte},
  [CMD_EEPROM_SAVE]     = { 0, 0, &dummy_byte},
  [CMD_POWER_OFF_0]     = { 0, 0, &dummy_byte},
  [CMD_POWER_OFF_1]     = { 0, 0, &dummy_byte},
  [CMD_EXT_RESET]       = { 0, 0, &dummy_byte},
  [CMD_EEPROM_READ]     = { 0, 8, &dummy_byte},
  [CMD_EEPROM_WRITE]    = { 8, 0, &dummy_byte},
  [CMD_GET_RESTARTS]    = { 1, 0, (uint8_t *)&restarts.internal},
};

uint8_t cmd_get_rx_len(uint8_t cmd)
{
  uint8_t len = 0;
  if (cmd < CMD_NBR_OF) len = cmd_data[cmd].rx_bytes;
  return len;
}
uint8_t cmd_get_tx_len(uint8_t cmd)
{
  uint8_t len = 0;
  if (cmd < CMD_NBR_OF) len = cmd_data[cmd].tx_bytes;
  return len;
}

uint8_t *cmd_get_data_ptr(uint8_t cmd)
{
  uint8_t *ptr = NULL;
  if (cmd < CMD_NBR_OF) ptr = cmd_data[cmd].data;
  return ptr;
}


void cmd_execute_cmd(uint8_t cmd)
{
  // io_blink_color_times(PIN_PWR_OFF, cmd, 2);
  switch(cmd)
  {
    case CMD_WD_UNDEF:
      break;
    case CMD_SET_WD_INTERVAL:
      main_data.wd_interval_ms = buff_get_u32((uint8_t*)i2c_buff.cmd, 1);
      main_data.wd_is_active = 1;
      edog_clear();
      //uint32_t buff_get_u32(uint8_t *buff, uint8_t indx)
      eep_req_save(EEPROM_MAIN_DATA);
      break;
    case CMD_GET_WD_INTERVAL:
      break;
    case CMD_SET_SLEEP_TIME:
      main_data.sleep_time_ms = buff_get_u32((uint8_t*)i2c_buff.cmd, 1);
      main_data.sleep_time_cycles = main_data.sleep_time_ms / 500;
      main_data.goto_sleep  = true;
      break;
    case CMD_GET_SLEEP_TIME:
      break; 
    case CMD_CLEAR_WATCHDOG:
      edog_clear();
      break;
    case CMD_SWITCH_OFF:
      break; 
    case CMD_SET_EEPROM_INDEX:
      user_eeprom_indx = i2c_buff.cmd[1];
      //io_blink_color_times(PIN_PWR_OFF_0, user_eeprom_indx, 2);
      break;
    case CMD_EEPROM_LOAD:
      //io_blink_color_times(PIN_PWR_OFF_0, user_eeprom_indx, 4);
      eep_load_array((eeprom_index_et)user_eeprom_indx, 8, i2c_buff.wrk );
      // i2c_buff.wrk[0] = 0xFE;
      // i2c_buff.wrk[1] = 0xDC;
      // tx_len = 8;
      break;   
    case CMD_EEPROM_SAVE:
      break;    
    case CMD_POWER_OFF_0:
      main_data.sleep_time_ms = buff_get_u32((uint8_t*)i2c_buff.cmd, 1);
      main_data.sleep_time_cycles = main_data.sleep_time_ms / 500;
      //main_data.goto_sleep = true;
      break; 
    case CMD_EXT_RESET:
      break;  
    case CMD_EEPROM_READ:
      break; 
    case CMD_EEPROM_WRITE:
      //io_blink_color_times(PIN_PWR_OFF_0, user_eeprom_indx, 6);
      eep_save_array((eeprom_index_et)user_eeprom_indx, 8, &i2c_buff.rx[1] );
      break; 
    case CMD_GET_RESTARTS:
      break; 

  }
}

void cmd_get_data(uint8_t cmd)
{
  uint8_t tx_len = 0;
  switch(cmd)
  {
    case CMD_WD_UNDEF:
      break;
    case CMD_SET_WD_INTERVAL:
       break;
    case CMD_GET_WD_INTERVAL:
      buff_set_u32((uint8_t*)i2c_buff.tx, 0, main_data.wd_interval_ms);  
      // buff_set_u32((uint8_t*)i2c_buff.tx, 0, 0x11223344);  
      tx_len = 4;  
      break;
    case CMD_SET_SLEEP_TIME:
      break;
    case CMD_GET_SLEEP_TIME:
      buff_set_u32((uint8_t*)i2c_buff.tx, 0, main_data.wd_interval_ms);  
      tx_len = 4;  
      break;
    case CMD_CLEAR_WATCHDOG:
      break;
    case CMD_SWITCH_OFF:
      break; 
    case CMD_SET_EEPROM_INDEX:
      break;
    case CMD_EEPROM_LOAD:
      break;   
    case CMD_EEPROM_SAVE:
      break;    
    case CMD_POWER_OFF_0:
      break; 
    case CMD_EXT_RESET:
      break;  
    case CMD_EEPROM_READ:
      memcpy(i2c_buff.tx, i2c_buff.wrk,8);
      // io_blink_color_times(PIN_PWR_OFF, cmd, 2);
      //eep_load_array(EEPROM_USER_1, 8, (uint8_t*)i2c_buff.tx );
      tx_len = 8;
      break; 
    case CMD_EEPROM_WRITE:
      break; 
    case CMD_GET_RESTARTS:
      i2c_buff.tx[0] = restarts.internal; 
      /// TODO 
      tx_len = 1;  

      break; 
  }

  if (tx_len > 0)
  {
    Wire.write((uint8_t *)i2c_buff.tx, tx_len); 
  }
  // io_blink_color_times(PIN_PWR_OFF_0, cmd, 2);
}


// This code i2c_request_event() is not working
// while (Wire.availableForWrite() && current_reg < REG_LEN) {
//     Wire.write(reg_bulk[current_reg++]);
// }
