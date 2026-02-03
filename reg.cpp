#include "Arduino.h"
#include "main.h"
#include "reg.h"
//#include "ee_prom.h"
//#include "helper.h"

uint8_t reg_bulk[REG_LEN];

//reg_data_st reg = {0};


void reg_initialize(void)
{
    reg_write_u32(REG_WD_INTERVAL, 70000);
}

uint8_t reg_get_item_len(uint8_t reg_addr)
{   uint8_t len = 0;
    switch(reg_addr)
    {
        case REG_WD_INTERVAL:   len = 4; break;
        case REG_SLEEP_TIME:    len = 4; break;
        case REG_WD_OFF_TIME:   len = 4; break;
        case REG_LOAD_SW:       len = 1; break;
        case REG_EEPROM_STATE:  len = 1; break;
        case REG_EEPROM_ADDR:   len = 1; break;
        case REG_CLR_WD:        len = 1; break;
        case REG_RESERVED_1:    len = 0; break;
        case REG_EEPROM_BUFF:   len = 16; break;
    }
    return len;
}

