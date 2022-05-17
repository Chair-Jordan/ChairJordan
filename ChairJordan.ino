#include "capacitance.h"
#include "weight.h"
#include "comms.h"

// unit: ms
#define CAP_READ_TIMEOUT    1000
#define CAP_CAL_CNT         20

#define WEIGHT_READ_TIMEOUT 1000
#define WEIGHT_CAL_CNT      20

#define LED 19

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  cap_setup();
  cap_recalibrate(CAP_CAL_CNT, CAP_READ_TIMEOUT);

  weight_setup();
  // Skip some of the first ones
  weight_recalibrate(WEIGHT_CAL_CNT, WEIGHT_READ_TIMEOUT);
  weight_recalibrate(WEIGHT_CAL_CNT, WEIGHT_READ_TIMEOUT);
  weight_recalibrate(WEIGHT_CAL_CNT, WEIGHT_READ_TIMEOUT);

  comms_setup();
}

void loop() {
  uint32_t timestamp = micros();
  int32_t cap = cap_read(CAP_READ_TIMEOUT);
  int32_t weight = weight_read(WEIGHT_READ_TIMEOUT);

  Serial.printf("time: %ld, cap: %ld, weight: %ld\n", timestamp, cap, weight);
  Serial.println(weight);

  blink(LED, 200);
}

void blink(int pin, int ms) {
  digitalWrite(pin, HIGH);
  delay(ms);
  digitalWrite(pin, LOW);
  delay(ms);
}
