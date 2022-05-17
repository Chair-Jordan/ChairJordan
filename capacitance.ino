/*
 * This file contains the logic for measuring capacitance
 */

#include "capacitance.h"
#include "window.h"

int32_t CAP_OVERHEAD = 0; // Base value for measurments
int32_t CAP_WINDOW[CAP_WINDOW_SIZE]; // Calculate average using sliding window

void cap_setup() {
  pinMode(PIN_CAP_CHARGE, OUTPUT);
  pinMode(PIN_CAP_OPAMP, INPUT);
}

// Measure capacitance from the circuit
// It works by discharging the capacitor and measuring the time it takes to charge it to 50%
// From this time value a capacitance is calculated
int32_t cap_read_once(uint32_t timeout) {
	// Discharge capactior
  digitalWrite(PIN_CAP_CHARGE, LOW);
  delay(100);
  uint32_t start = micros();
  digitalWriteFast(PIN_CAP_CHARGE, HIGH);

  // Wait for charge to read 50%
  while (digitalReadFast(PIN_CAP_OPAMP) == LOW && micros() < start + timeout);
	// Calculate capacitance from charge time
  uint32_t diff = micros() - start;
  float RC = (-(float)diff)/(logf((CAP_VREF - CAP_VCC)/(CAP_V0 - CAP_VCC)));
  RC *= 1000000; // us -> s
  int32_t C = RC/(float)CAP_R;
  Serial.printf("raw C: %ld\n", C);

  return C - CAP_OVERHEAD;
}

// Measure capacitance, add the value to the sliding window and return the average of the sliding window
int32_t cap_read(uint32_t timeout) {
  int32_t C = cap_read_once(timeout);
  window_insert(CAP_WINDOW, CAP_WINDOW_SIZE, C);
  Serial.println("Cap window");
  for (int i = 0; i < CAP_WINDOW_SIZE; i++)
    Serial.printf("%ld ", CAP_WINDOW[i]);
  Serial.println("");
  Serial.printf("cap zero: %ld\n", CAP_OVERHEAD);
  return window_avg(CAP_WINDOW, CAP_WINDOW_SIZE);
}

// Set CAP_OVERHEAD variable
// This sets the "base value" of measurements
void cap_recalibrate(int measurements, uint32_t timeout) {
  CAP_OVERHEAD = 0;
  int32_t sum = 0;
  for (int i = 0; i < measurements; i++) {
    sum += cap_read_once(timeout);
  }
  CAP_OVERHEAD = sum/measurements;
}
