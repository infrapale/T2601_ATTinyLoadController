#ifndef __MAIN_H__
#define __MAIN_H__

#define  I2C_ADDR           (0x24)
#define  TICK_TIME          100


typedef struct
{
  uint32_t wd_next_hit_ms;
  uint32_t wd_power_off_ms; 
  // uint8_t  wd_is_active;
  uint32_t sleep_time_cycles;     //approx 500ms
  //bool     goto_sleep;
  bool     do_wakeup_routines;
  //uint32_t sleep_time_ms;
} main_data_st;

typedef struct
{
  uint16_t internal;
  uint16_t sleep;
  uint16_t watchdog;
} restarts_st;



#endif