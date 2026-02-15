#include "Arduino.h"
#include <avr/sleep.h>
#include <avr/power.h>
#include "main.h"
#include "io.h"



void io_initialize(void)
{
  io_gpio_enable();
  io_n_pulses(4, 20);
}


void io_n_pulses(uint8_t n, uint16_t us)
{
  for (uint8_t i=0; i < n; i++)
  {
    io_test_on();
    delayMicroseconds(us);
    io_test_off();
    delayMicroseconds(us);
  }
}


void io_power1_off(void)
{
    PORTA.DIRCLR = PIN_OUT_PWR1_OFF_BM;
}

void io_power1_on(void)
{
    PORTA.DIRSET = PIN_OUT_PWR1_OFF_BM;
    PORTA.OUTCLR = PIN_OUT_PWR1_OFF_BM;
}

void io_power2_off(void)
{
    PORTA.DIRCLR = PIN_OUT_PWR2_OFF_BM;
}

void io_power2_on(void)
{
    PORTA.DIRSET = PIN_OUT_PWR2_OFF_BM;
    PORTA.OUTCLR = PIN_OUT_PWR2_OFF_BM;
}


bool io_inp_clr_wd(void)
{
    //bool wd  = ((PORTA.IN & PIN_INP_CLR_WD_BM) != 0);
    return true;
}


void io_gpio_enable(void)
{
  //PORTA.DIRSET = PIN_OUT_PWR_OFF_BM;
  //PORTA.OUTSET = PIN_OUT_PWR_OFF_BM;

  PORTA.DIRSET = PIN_OUT_TEST_BM;
  PORTA.OUTSET = PIN_OUT_TEST_BM;
  PORTA.DIRSET = PIN_OUT_PWR1_OFF_BM;
  PORTA.DIRSET = PIN_OUT_PWR2_OFF_BM;
  PORTA.OUTCLR = PIN_OUT_PWR1_OFF_BM;
  PORTA.OUTCLR = PIN_OUT_PWR2_OFF_BM;


  io_test_on();

  //PORTA.DIRCLR = PIN_INP_CLR_WD_BM;
  PORTA.PIN6CTRL =  (PORT_PULLUPEN_bm | PORT_ISC_INTDISABLE_gc) & ~PORT_ISC_INPUT_DISABLE_gc;  
}

// bool io_inp_goto_sleep(void)
// {
//     bool zzz  = (~PORTA.IN & PIN_INP_SLEEP_BM);
//     return zzz;
// }


//////////////////////////////////////////////////////////////////
//  disableAllPeripherals()
//
//  To achieve minimum current consumption during sleep, it's best to
//  disable everything that might draw current during sleep.
//  This function disables everything except the RTC-PIT and 32K
//  internal low power oscillator.  This function does not disable
//  the counter used by the Arduino framework to implement millis(),
//  although the clock that drives that counter is suspended during sleep.
//
void io_gpio_disable(void) {
  PORTA.DIRCLR = PIN0_bm; //  set port A0 to input
  PORTA.DIRCLR = PIN1_bm; //  set port A1 to input
  PORTA.DIRCLR = PIN2_bm; //  set port A2 to input
  PORTA.DIRCLR = PIN3_bm; //  set port A3 to input
  PORTA.DIRCLR = PIN6_bm; //  set port A4 to input
  PORTA.DIRCLR = PIN7_bm; //  set port A5 to input        
    
  PORTA.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc; // disable input buffers
  PORTA.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc;
  PORTA.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
  PORTA.PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc;
  PORTA.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc;
  PORTA.PIN7CTRL = PORT_ISC_INPUT_DISABLE_gc;
  
  //USART0.CTRLB &= ~USART_RXEN_bm & ~USART_TXEN_bm; // disable USART
}

#define PA3_INTERRUPT PORTA.INTFLAGS & PIN3_bm
#define PA3_CLEAR_INTERRUPT_FLAG PORTA.INTFLAGS &= PIN3_bm

ISR(PORTA_PORT_vect)
{
  if(PA3_INTERRUPT)
  {
    //pb2Ioc = 1;
    PA3_CLEAR_INTERRUPT_FLAG;
  }
}

//  LED control functions
void io_test_on(void) {PORTA.OUTSET = PIN_OUT_TEST_BM;} 
void io_test_off(void) {PORTA.OUTCLR = PIN_OUT_TEST_BM;}
void io_test_toggle(void) {PORTA.OUTTGL = PIN_OUT_TEST_BM;} 



