#ifndef __REG_H__
#define __REG_H__
#include "main.h"


#define  REG_LEN                64
#define  REG_EEPROM_BUFF_LEN    16
#define  REG_NBR_LOAD_SW        2

typedef struct
{
    uint32_t  *wd_interval;
    uint32_t  *sleep_time;
    uint8_t   *load_sw;  
    uint8_t   *eeprom_state;
    uint8_t   *eeprom_addr;
    uint8_t   *reserved;
    uint8_t   *eeprom_buff;
} reg_data_st;

typedef enum
{
    REG_WD_INTERVAL     = 0,
    REG_SLEEP_TIME      = REG_WD_INTERVAL + 4,
    REG_LOAD_SW         = REG_SLEEP_TIME + 4,
    REG_EEPROM_STATE    = REG_LOAD_SW + REG_NBR_LOAD_SW,
    REG_EEPROM_ADDR     = REG_EEPROM_STATE + 1,
    REG_RESERVED_1      = REG_EEPROM_ADDR + 1,
    REG_EEPROM_BUFF     = REG_RESERVED_1 + 4
} reg_data_et;

void reg_initialize(void);
uint8_t reg_get_item_len(uint8_t reg_addr); 

/*

typedef enum
{
  I2C_CMD_WD_UNDEF         = 0x00,
  I2C_CMD_SET_WD_INTERVAL  = 0x01,
  I2C_CMD_GET_WD_INTERVAL  = 0x02,
  I2C_CMD_SET_SLEEP_TIME   = 0x03,
  I2C_CMD_RD_SLEEP_TIME    = 0x04,
  I2C_CMD_CLEAR_WATCHDOG   = 0x05,
  I2C_CMD_SWITCH_OFF       = 0x06,
  I2C_CMD_SET_EEPROM_ADDR  = 0x07,
  I2C_CMD_EEPROM_LOAD      = 0x08
  I2C_CMD_EEPROM_SAVE      = 0x09,
  I2C_CMD_POWER_OFF_0      = 0x0A,
  I2C_CMD_EXT_RESET        = 0x0B,
  I2C_CMD_EEPROM_READ      = 0x0C,
  I2C_CMD_EEPROM_WRITE     = 0x0D,
  I2C_CMD_GET_RESTARTS     = 0x0E,
 } i2c_cmd_et;


typedef struct
{
  uint8_t tx_bytes;
  uint8_t rx_bytes;
  uint8_t *data;
} i2c_data_st;

typedef enum 
{
  MASTER_TO_SLAVE = 0,
  SLAVE_TO_MASTER 
} master_slave_direction_et;


typedef struct
{
    uint8_t   state;
    uint32_t  timeout_ms;
    uint32_t  interval_ms;
    uint32_t  next_reset_ms;
    uint32_t  next_forced_reset_ms;
    bool      is_active;
} wd_st;


void reg_initialize(void);

void reg_warm_setup(void);

uint32_t reg_rd_u32(uint8_t reg_addr);

void reg_wr_u32(uint8_t reg_addr, uint32_t u32);

uint8_t reg_get_msg_len(I2C_CMND_et reg_addr, master_slave_direction_et master_slave);

void reg_action_on_receive(I2C_CMND_et reg_addr);

void reg_wd_state_machine(void);

void reg_time_machine(void);
*/
#endif