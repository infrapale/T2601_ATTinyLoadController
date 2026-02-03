/*
my_eeprom  
EEPROM Memory Map and Functions

https://docs.arduino.cc/learn/built-in-libraries/eeprom/

    EEPROM Write:

---->(EEPROM_IDLE)
|          |   [REG_EEPROM_BUFF] = data
|          |   [REG_EEPROM_STATUS] = EEPROM_WR_BUFF
|          V
|    (EEPROM_WR_BUFF)
|          | 
|    (EEPROM_READY)      |  
|          |
|          |
|<----------  if (EEPROM_READY) [REG_EEPROM_STATUS] = EEPROM_IDLE



    EEPROM Read:

---->(EEPROM_IDLE)
|          |
|          |   [REG_WR_EEPROM]
|          |   [REG_EEPROM_STATUS] = EEPROM_RD_BUFF
|          V
|    (EEPROM_RD_BUFF)
|          |
|         < >  if (eprom_rd_ready) [REG_EEPROM_STATUS] = EEPROM_READY
|          V
|    (EEPROM_READY)
|          |
|          |   data = [REG_EEPROM_BUFF]
|          |   [REG_EEPROM_STATUS] = EEPROM_IDLE
------------




*/


#ifndef __MY_EEPROM_H__
#define __MY_EEPROM_H__

#define ATTINY412_EEPROM_SIZE 128

typedef enum 
{
    EEPROM_IDLE = 0,
    EEPROM_WR_BUFF,
    EEPROM_RD_BUFF,
    EEPROM_READY,
} eeprom_state_et;


#define EEPROM_ADDR_MAIN_DATA     0x00
#define EEPROM_ADDR_RESTARTS      0x20
#define EEPROM_ADDR_USER          0x30


void epp_initialize_data(void);

void eep_state_machine(void);

uint8_t eep_load_u8(uint8_t addr);

void eep_save_u8(uint8_t addr, uint8_t u8 );

void eep_save_array(uint8_t addr, uint8_t bytes, uint8_t *u8_arr );

void eep_load_array(uint8_t addr, uint8_t bytes, uint8_t *u8_arr );


#endif
