#ifndef __EDOG_H__
#define __EDOG_H__

void edog_clear();

void edog_force_reset(uint16_t reset_duration);

void edog_initialize(void);

void edog_state_machine(void);

#endif