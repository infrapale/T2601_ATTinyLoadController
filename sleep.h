#ifndef __SLEEP_H__
#define __SLEEP_H__



void sleep_setup(void);

void init32kOscRTCPIT(void);

void initSleepMode(void);

void sleepNCycles(uint8_t val);

void disableAllPeripherals(void);

void ledOn(void);
void ledOff(void);
void pwr_off(void);
void pwr_on(void);

void ledToggle(void);
void diagnosticPinHigh(void);
void diagnosticPinLow(void);
void diagnosticPinToggle(void);

void initSerialGPIO(void);

void flashByte(uint8_t val);
  
#endif