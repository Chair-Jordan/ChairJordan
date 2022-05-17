/*
 * Contains the logic for measuring weight using load cells and a HX711 module
*/

#include "weight.h"
#include "window.h"

#define HX711_GAIN 1

int32_t WEIGHT_WINDOW[WEIGHT_WINDOW_SIZE]; // Calculate the average using a sliding window
int32_t WEIGHT_ZERO = 0; // Base value for measurments

void weight_setup(void) {
  pinMode(PIN_WEIGHT_SCK, OUTPUT);
  pinMode(PIN_WEIGHT_DT, INPUT);

  digitalWrite(PIN_WEIGHT_SCK, LOW);
}

// Read one bit from the HX711 module
// A new bit is presented for each clock tick
int weight_read_bit() {
  digitalWrite(PIN_WEIGHT_SCK, HIGH);
  delayMicroseconds(1);
  digitalWrite(PIN_WEIGHT_SCK, LOW);

  return digitalRead(PIN_WEIGHT_DT);
}

// Read one measurment from amplifier chip
// Reads 24 bits from the HX711 module
int32_t weight_read_hx711(void) {
  // Wait until data is available
  while (digitalRead(PIN_WEIGHT_DT));

  uint32_t weight = 0;
  // Read data, the chip outputs 24 bit values
  for (int i = 0; i < 24; i++) {
    weight <<= 1;
    weight |= weight_read_bit();
  }
  Serial.printf("weight: %ld\n", weight);
  // Skip uneccesary bits
  for (int i = 0; i < HX711_GAIN; i++) {
    weight_read_bit();
  }

  weight ^= 0x800000; // Invert the value
  Serial.printf("weight: %ld\n", weight);

  return weight - WEIGHT_ZERO;
}

// Find a new value for WEIGHT_ZERO
void weight_recalibrate(int measurements, uint32_t timeout) {
  WEIGHT_ZERO = 0;

  int32_t sum = 0;
  for (int i = 0; i < measurements; i++)
    sum += weight_read_hx711();

  WEIGHT_ZERO = sum/measurements;
}

// Read a value, add it to the sliding window and return the average of the sliding window
int32_t weight_read(uint32_t timeout) {
  int32_t weight = weight_read_hx711();
  window_insert(WEIGHT_WINDOW, WEIGHT_WINDOW_SIZE, weight);
  Serial.println("Weight window");
  for (int i = 0; i < WEIGHT_WINDOW_SIZE; i++)
    Serial.printf("%ld ", WEIGHT_WINDOW[i]);
  Serial.println("");
  Serial.printf("zero: %ld\n", WEIGHT_ZERO);
  return window_avg(WEIGHT_WINDOW, WEIGHT_WINDOW_SIZE);
}
