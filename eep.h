/*
my_eeprom  
EEPROM Memory Map and Functions

https://docs.arduino.cc/learn/built-in-libraries/eeprom/

*/


#ifndef __MY_EEPROM_H__
#define __MY_EEPROM_H__

#define ATTINY412_EEPROM_SIZE 128

#define EEPROM_ADDR_MAIN_DATA     0x00
#define EEPROM_ADDR_RESTARTS      0x20
#define EEPROM_ADDR_USER_0        0x30
#define EEPROM_ADDR_USER_1        0x38
#define EEPROM_ADDR_USER_2        0x50
#define EEPROM_ADDR_USER_3        0x58
#define EEPROM_ADDR_USER_4        0x60
#define EEPROM_ADDR_USER_5        0x68
#define EEPROM_ADDR_USER_6        0x70
#define EEPROM_ADDR_USER_7        0x78

typedef enum
{
  EEPROM_MAIN_DATA     = 0,
  EEPROM_RESTARTS,
  EEPROM_USER_0,
  EEPROM_USER_1,
  EEPROM_USER_2,
  EEPROM_USER_3,
  EEPROM_USER_4,
  EEPROM_USER_5,
  EEPROM_USER_6,
  EEPROM_USER_7,
  EEPROM_NBR_OF
 } eeprom_index_et;

void epp_initialize_data(void);

void eep_req_save(uint8_t eep_indx);

uint8_t eep_load_u8(eeprom_index_et indx, int offs);

void eep_save_array(eeprom_index_et indx, uint8_t bytes, uint8_t *u8_arr );

void eep_load_array(eeprom_index_et indx, uint8_t bytes, uint8_t *u8_arr );

void eep_time_machine(void);


#endif
