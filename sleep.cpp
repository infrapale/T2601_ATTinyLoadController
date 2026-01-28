/****************************************************************************************
Sleep function:

I2C  | | |Set sleep time| | |

Sleep Request   |H|H|H|H|H|H|H|L|L|*|*|*|*|*|*|*|H|H|H|H|H|H|H|H|H|             

Power Off       |H|H|H|H|H|H|H|H|H|L|L|L|L|L|L|L|H|H|H|H|H|H|H|H|H|             
             
Sleep Time                         <------------>


*****************************************************************************************
                ---------------------
                |                   |
    3.3-5V    --| (1) VDD   GND (8) |-- GND
                |                   |
    xxx       --| (2) PA6   PA3 (7) |-- SLEEP
                |                   |
    PWR_OFF   --| (3) PA7   PA0 (6) |-- UPDI
                |                   |
    I2C SDA   --| (4) PA1   PA2 (5) |-- I2C SCL
                |                   |
                 ---------------------
                                               

*****************************************************************************************
https://ww1.microchip.com/downloads/en/Appnotes/90003229A.pdf
https://github.com/microPaul/ATtiny412-Sleeping-Lighthouse/blob/main/main.c
****************************************************************************************/


#include "Arduino.h"
#include <avr/sleep.h>
#include "sleep.h"
#include "io.h"


volatile uint8_t rtcIntSemaphore;  // flag from RTS interrupt that may be used by polled function


///////////////////////////////////////////////////////////
// setup
//
// This is the "run it once at the start" code to setup the system.
//
void sleep_setup(void) {
  //Serial.swap(1); // need UART pins in swap position to leave DAC available
  io_gpio_enable();
  //initSerialGPIO(); // initialize serial and GPIO (serial is commented out)
  //Serial.println(); // print blank line
  //Serial.println("Sleeping Lighthouse signon msg"); 

  init32kOscRTCPIT(); // init the 32K internal Osc and RTC-PIT for interrupts
  initSleepMode();  // set up the sleep mode

  // now show some diagnostic data to confirm that the MCU system
  // speed is 4 MHz and not 20 MHz.  At 4 MHz the MCU should run
  // correctly from 1.8 VDC to 5.5 VDC.
  //
  //  delay(1000);
  //  flashByte(FUSE.OSCCFG);  // flash the value, should be 0x01 for 16 MHz internal oscillator
  //  delay(2000);
  //  flashByte(CLKCTRL.MCLKCTRLB); // flash the value, should be 0x03 for divide clock by 4
  //  delay(2000);
 
}



//////////////////////////////////////////////////////////////////
//  ISR(RTC_PIT_vect)
//
//  Interrupt Service Routine for the RTC PIT interrupt
//
ISR(RTC_PIT_vect) {
  RTC.PITINTFLAGS = RTC_PI_bm;  // clear the interrupt flag   
  rtcIntSemaphore = 1; // mark to polled function that interrupt has occurred

}




//////////////////////////////////////////////////////////////////
//  init32kOscRTCPIT()
//
//  initialize the internal ultra low power 32 kHz osc and periodic Interrupt timer
//
//  these two peripherals are interconnected in that the internal 32 kHz
//  osc will not start until a peripheral (PIT in this case) calls for it,
//  and the PIT interrupt should not be enabled unit it is confirmed that
//  the 32 kHz osc is running and stable
//
//  Note that there is ERRATA on the RTC counter,  see:
//  https://ww1.microchip.com/downloads/en/DeviceDoc/ATtiny212-214-412-414-416-SilConErrataClarif-DS80000933A.pdf
//
//  That document currently states "Any write to the RTC.CTRLA register resets the 15-bit prescaler
//  resulting in a longer period on the current count or period".  So if you load the prescaler
//  value and then later enable the RTC (both on the same register) you get a very long
//  (max?) time period.  My solution to this problem is to always enable the RTC in the same
//  write that sets up the prescaler.  That seems to be an effective work-around. 
//
void init32kOscRTCPIT(void) {
  _PROTECTED_WRITE(CLKCTRL.OSC32KCTRLA, CLKCTRL_RUNSTDBY_bm);  // enable internal 32K osc   
  RTC.CLKSEL = RTC_CLKSEL_INT1K_gc; //Select 1.024 kHz from 32KHz Low Power Oscillator (OSCULP32K) as clock source
  RTC.PITCTRLA = RTC_PERIOD_CYC512_gc | RTC_PITEN_bm; //  Enable RTC-PIT with divisor of 512 (~500 milliseconds)
  while (!(CLKCTRL.MCLKSTATUS & CLKCTRL_OSC32KS_bm));  // wait until 32k osc clock has stabilized
  while (RTC.PITSTATUS > 0); // wait for RTC.PITCTRLA synchronization to be achieved
  RTC.PITINTCTRL = RTC_PI_bm;  // allow interrupts from the PIT device  
}

//////////////////////////////////////////////////////////////////
//  initSleepMode()
//
//  this doesn't invoke sleep, it just sets up the type of sleep mode
//  to enter when the MCU is put to sleep by calling "sleep_cpu()"
//
void initSleepMode(void) {
  //SLPCTRL.CTRLA = SLPCTRL_SMODE_IDLE_gc; // set sleep mode to "idle"
  //SLPCTRL.CTRLA = SLPCTRL_SMODE_STDBY_gc; // set sleep mode to "standby"  
  
  SLPCTRL.CTRLA = SLPCTRL_SMODE_PDOWN_gc; // set sleep mode to "power down"
  SLPCTRL.CTRLA |= SLPCTRL_SEN_bm;  // enable sleep mode
  //set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  //sleep_enable(); 
}


//////////////////////////////////////////////////////////////////
//  sleepNCycles(uint16_t val)
//
//  cause system to go to sleep for N cycles, where each cycle
//  is about 500mS.  Note that processor does wakeup every 500 ms
//  but goes back to sleep almost immediately if it has not yet
//  done the desired number of cycles.
//
//  At 4 MHz MCU speed, the awake duration of each iternation in the
//  for loop below is approximately 1.5 microseconds.
//
void sleepNCycles(uint8_t val) {
  // first cycle may not be full cycle
  io_gpio_disable(); // all off
  for (uint8_t i = 0; i < val ; i++) {
    //diagnosticPinLow(); // set low to show sleeping
    sleep_cpu();  // put MCU to sleep
    //diagnosticPinHigh(); // set high to show awake
  }
  // now awake, sleep cycles complete, continue on
  io_gpio_enable();
  initSleepMode();
  //initSerialGPIO(); // initialize serial and GPIO 
}



//////////////////////////////////////////////////////////////////
//  initSerialGPIO()
//
//  initialize Serial port and all needed GPIO
//
void xxinitSerialGPIO(void) {
  //Serial.begin(9600, SERIAL_8N1); 
  PORTA.DIRSET = PIN_OUT_PWR_OFF_BM; //  set port to output for LED
  PORTA.OUTCLR = PIN_OUT_PWR_OFF_BM; // turn off LED
  PORTA.DIRSET = PIN2_bm; //  set port A2 to output for scope diagnostic
  PORTA.OUTCLR = PIN2_bm; // set A2 output low
}



//////////////////////////////////////////////////////////////////
//  flashByte
//
//  This function provides a diagnosic report via a single LED.
//  An LED is flashed short when a bit is zero and flashed long when
//  a bit is one.  Eight bits are displayed sequentially starting with
//  LSB and ending with MSB.
//
void flashByte(uint8_t val) {
  uint8_t i;
  for (i = 0; i < 8 ; i++) {
    ledOn(); // turn on LED
    (val & 1) ? delay(700) : delay (100);  // long delay for 1, short if zero
    ledOff(); // turn off LED
    delay(1000);
    val >>= 1; // right shift val by one bit
  }
}

