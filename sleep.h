#ifndef __SLEEP_H__
#define __SLEEP_H__

typedef enum 
{
    SLEEP_INACTIVE = 0,
    SLEEP_ACTIVATED,
    SLEEP_ZZZZZ,
} sleep_state_et;


void sleep_setup(void);

void init32kOscRTCPIT(void);

void initSleepMode(void);

void sleepNCycles(uint8_t val);

void disableAllPeripherals(void);

void sleep_state_machine(void);

//void initSerialGPIO(void);


#endif