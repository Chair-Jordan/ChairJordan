/*
 * This file contains the code for reducing power consumption
 * Based on the example on this page:
 * https://github.com/SpenceKonde/DxCore/blob/0c2ed7c64b8991d4f8c956fde49bdce861fc6ed1/megaavr/extras/PowerSave.md
 */
#include "avr/sleep.h"

// Initialize RTC (Clock)
void RTC_init(void)
{
  while (RTC.STATUS > 0){;} // Wait for all register to be synchronized
  
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
void pwr_init() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set which sleep mode to use
  RTC_init(); // Initialize the RTC timer
  sleep_enable(); // Enable sleep_cpu()
  pinMode(PIN_RADIO_RESET, OUTPUT);
  pwr_RF_sleep();
}

// Wake and sleep radio by setting reset pin to high/low
void pwr_RF_sleep() {
  radio.sleep();
  digitalWrite(PIN_RADIO_RESET, HIGH);
}
void pwr_RF_wake() {
  digitalWrite(PIN_RADIO_RESET, LOW);
}
// Sleep for amount amount of interrupts
void pwr_cpu_sleep(int amount) {
  int i = amount;
  while(i >= 0) {
    sleep_cpu();
    i = i - 1;
  }
}
static bool pwr_check_over_threshold(int capacitance, int weight) {
  return (cap_read_once(CAP_READ_TIMEOUT) > capacitance | weight_read_hx711() > weight);
}
// Check against threshold three times, if all are above return true
bool pwr_should_sleep(int cycles) {
  for(int i = 0; i < 3; i++) {
    if (!pwr_check_over_threshold(CAP_WAKEUP_VAL, WEIGHT_WAKEUP_VAL)) // Check red value against threshold
      return true;
    pwr_cpu_sleep(cycles); // If not over threshold sleep and try again
  }
  return false;
}

// Idle and make measurment every cycles interrupt (one is about 1 second I think, double check this)
// Return when a value above threshold is measured
void pwr_idle(int cycles) {
  while(pwr_should_sleep(cycles)) {
    pwr_cpu_sleep(cycles);
  }
}
