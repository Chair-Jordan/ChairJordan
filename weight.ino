#include "weight.h"
#include "window.h"

#define HX711_GAIN 1

int32_t WEIGHT_WINDOW[WEIGHT_WINDOW_SIZE];
int32_t WEIGHT_ZERO = 0;

void weight_setup(void) {
  pinMode(PIN_WEIGHT_SCK, OUTPUT);
  pinMode(PIN_WEIGHT_DT, INPUT);

  digitalWrite(PIN_WEIGHT_SCK, LOW);
}

int weight_read_bit() {
  digitalWrite(PIN_WEIGHT_SCK, HIGH);
  delayMicroseconds(1);
  digitalWrite(PIN_WEIGHT_SCK, LOW);

  return digitalRead(PIN_WEIGHT_DT);
}

// Read one measurment from amplifier chip
int32_t weight_read_hx711(void) {
  // Wait until data is available
  while (digitalRead(PIN_WEIGHT_DT));

  uint32_t weight = 0;
  // Read data
  for (int i = 0; i < 24; i++) {
    weight <<= 1;
    weight |= weight_read_bit();
  }
  Serial.printf("weight: %ld\n", weight);
  // Skip uneccesary bits
  for (int i = 0; i < HX711_GAIN; i++) {
    weight_read_bit();
  }
  // Changes the range or some shit
  weight ^= 0x800000;
  Serial.printf("weight: %ld\n", weight);

  return weight - WEIGHT_ZERO;
}

void weight_recalibrate(int measurements, uint32_t timeout) {
  WEIGHT_ZERO = 0;

  int32_t sum = 0;
  for (int i = 0; i < measurements; i++)
    sum += weight_read_hx711();

  WEIGHT_ZERO = sum/measurements;
}

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
