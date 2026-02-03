#include <Arduino.h>
#include "main.h"
#include <EEPROM.h>
#include "eep.h"
#include "reg.h"
#include "io.h"

#define EEP_SAVE_INTERVAL_ms   1000
#define EEP_CNTR_START            4



uint32_t next_run = 0;

extern main_data_st main_data;
extern restarts_st restarts;
//extern uint8_t reg_bulk[];

void epp_initialize_data(void)
{
    reg_write_u8(REG_EEPROM_STATE, EEPROM_IDLE);
}


void eep_save_u8(uint8_t addr, uint8_t u8 )
{
  EEPROM.write(addr, u8);
}

uint8_t eep_load_u8(uint8_t addr  )
{
  return EEPROM.read(addr);
}

void eep_save_array(uint8_t addr, uint8_t bytes, uint8_t *u8_arr )
{
  for (uint8_t i = 0; i < bytes; i++)
  {
    EEPROM.write(addr++, u8_arr[i]);
  }
}

void eep_load_array(uint8_t addr, uint8_t bytes, uint8_t *u8_arr )
{
  for (uint8_t i = 0; i < bytes; i++)
  {
    u8_arr[i] = EEPROM.read(addr++);
  }
}


void eep_state_machine(void)
{
    switch(reg_read_u8(REG_EEPROM_STATE))
    {
        case EEPROM_IDLE:
            break; 
        case EEPROM_WR_BUFF:
            io_n_pulses(2, 20);
            eep_save_array(reg_read_u8(REG_EEPROM_ADDR), 16, &reg_bulk[REG_EEPROM_BUFF]);
            io_n_pulses(1, 10);
            reg_write_u8(REG_EEPROM_STATE, EEPROM_READY);
            break; 
        case EEPROM_RD_BUFF:
            io_n_pulses(3, 20);
            eep_load_array(reg_read_u8(REG_EEPROM_ADDR), 16, &reg_bulk[REG_EEPROM_BUFF]);
            io_n_pulses(1, 20);
            reg_write_u8(REG_EEPROM_STATE, EEPROM_READY);
            break; 
        case EEPROM_READY:
            break; 
    }
}

