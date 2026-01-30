#include <Arduino.h>
#include "main.h"
#include <EEPROM.h>
#include "eep.h"
#include "reg.h"

#define EEP_SAVE_INTERVAL_ms   1000
#define EEP_CNTR_START            4

extern reg_data_st reg;

typedef struct
{
  int  addr;
  uint8_t  save_cntr ;
} eep_st;

eep_st eep[EEPROM_NBR_OF] =
{
  [EEPROM_MAIN_DATA]  = {EEPROM_ADDR_MAIN_DATA, 0},
  [EEPROM_RESTARTS]   = {EEPROM_ADDR_RESTARTS, 0},
  [EEPROM_USER_0]     = {EEPROM_ADDR_USER_0, 0},
  [EEPROM_USER_1]     = {EEPROM_ADDR_USER_1, 0},
  [EEPROM_USER_2]     = {EEPROM_ADDR_USER_2, 0},
  [EEPROM_USER_3]     = {EEPROM_ADDR_USER_3, 0},
  [EEPROM_USER_4]     = {EEPROM_ADDR_USER_4, 0},
  [EEPROM_USER_5]     = {EEPROM_ADDR_USER_5, 0},
  [EEPROM_USER_6]     = {EEPROM_ADDR_USER_6, 0},
  [EEPROM_USER_7]     = {EEPROM_ADDR_USER_7, 0},
  
};

uint32_t next_run = 0;

extern main_data_st main_data;
extern restarts_st restarts;

void epp_initialize_data(void)
{
  for(uint8_t i = 0; i < EEPROM_NBR_OF; i++)
  {
    eep[i].save_cntr = EEP_CNTR_START;
  }
  next_run = millis() + EEP_SAVE_INTERVAL_ms;
  EEPROM.get(EEPROM_ADDR_MAIN_DATA, main_data);
  EEPROM.get(EEPROM_ADDR_RESTARTS, restarts);
}

void eep_req_save(uint8_t eep_indx)
{
  if(eep[eep_indx].save_cntr == 0) eep[eep_indx].save_cntr = EEP_CNTR_START;
}

void eep_save_u8(eeprom_index_et indx, int offs, uint8_t u8 )
{
  EEPROM.write(eep[indx].addr + offs, u8);
}

uint8_t eep_load_u8(eeprom_index_et indx, int offs  )
{
  return EEPROM.read(eep[indx].addr + offs);
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
  for (int i = 0; i < bytes; i++)
  {
    u8_arr[i] = EEPROM.read(addr++);
  }
}


void eep_state_machine(void)
{
    switch(*reg.eeprom_state)
    {
        case EEPROM_IDLE:
            break; 
        case EEPROM_WR_BUFF:
            eep_save_array(*reg.eeprom_addr, 16, reg.eeprom_buff);
            *reg.eeprom_state = EEPROM_IDLE;
            break; 
        case EEPROM_RD_BUFF:
            eep_load_array(*reg.eeprom_addr, 16, reg.eeprom_buff);
            *reg.eeprom_state = EEPROM_READY;
            break; 
        case EEPROM_READY:
            break; 
    }
}


void eep_time_machine(void)
{
  if (millis() > next_run)
  {
    next_run = millis() + EEP_SAVE_INTERVAL_ms; 
    for(uint8_t indx = 0; indx < EEPROM_USER_1; indx++)
    {
      if (eep[indx].save_cntr == 1 )
      {
          eep[indx].save_cntr = 0;
          switch(indx)
          {
            case EEPROM_MAIN_DATA:
              EEPROM.put(EEPROM_ADDR_MAIN_DATA, main_data);
              break;
            case EEPROM_RESTARTS:
              EEPROM.put(EEPROM_ADDR_RESTARTS, restarts);
              break;
          } 
      }
      else
      {
        eep[indx].save_cntr--;
      }  
    }
  }  
}
