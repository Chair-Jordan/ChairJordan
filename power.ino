/*
 * This file contains the code for reducing power consumption
 * Based on the example on this page:
 * https://github.com/SpenceKonde/DxCore/blob/0c2ed7c64b8991d4f8c956fde49bdce861fc6ed1/megaavr/extras/PowerSave.md
 */
#include "avr/sleep.h"

// Initialize RTC (Clock)
void RTC_init(void)
{
  while (RTC.STATUS > 0)
  {
    ;                                   // Wait for all register to be synchronized
  }
  RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;    // 32.768kHz Internal Ultra-Low-Power Oscillator (OSCULP32K)
  RTC.PITINTCTRL = RTC_PI_bm;           // PIT Interrupt: enabled 
  RTC.PITCTRLA = 0xF0 | RTC_PITEN_bm; // set prescale and enable clock. Prescale is 1/32768
}

// Not sure why this is here but it is needed according to the example
ISR(RTC_PIT_vect)
{
  RTC.PITINTFLAGS = RTC_PI_bm;          // Clear interrupt flag
}

// Initialize power saving stuff
void power_init() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set which sleep mode to use
  sleep_enable(); // Enable the sleep function
  RTC_init(); // Initialize the RTC timer
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // Set sleep mode to POWER DOWN mode
  sleep_enable(); // Enable sleep_cpu()
}

// Set unused pins to output
void init_pins() {
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  for (i = 8; i <= 17; i++)
    pinMode(i, OUTPUT); 
}

// Sleep for 1 interrupt
void sleep() {
	Serial.flush();
	sleep_cpu();
}
