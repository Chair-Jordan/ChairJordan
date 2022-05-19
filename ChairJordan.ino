/*
 * This file contains the main logic for the micro controller
 * Pin mapping: (MC = micro controller) (IO pins are numbered)
 *                               _____
 *           RF slave select    7|   |6    RF clock
 *                     unused   8|   |5    RF miso 
 *                 CAP charge   9|   |4    RF mosi
 *            CAP read opamp   10|   |3    RF reset
 *                    unused   11|   |2    RF dio0
 *          WEIGHT read data   12|   |1    MC rx
 *              WEIGHT clock   13|   |0    MC tx
 *                    unused   14|   |     MC gnd
 *                    unused   15|   |     MC vcc
 *                    unused   16|   |     unused
 *                    unused   17|   |22   MC reset
 *                    unused   18|   |21   unused
 *                    unused   19|   |20   unused
 * MC avcc (coupled with vcc)    |___|     MC gnd
 *  
 */

#include "capacitance.h"
#include "weight.h"
#include "comms.h"
#include "power.h"

// unit: ms
#define CAP_READ_TIMEOUT    1000
#define CAP_CAL_CNT         20

#define WEIGHT_READ_TIMEOUT 1000
#define WEIGHT_CAL_CNT      20

// Set all unused pins as output to lower power consumption
void init_pins() {
  // Debugging, disable all pins
  for(int i = 0; i <= 23; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  /*
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  */
}

void setup() {
  Serial.begin(9600);

  cap_setup();
  cap_recalibrate(CAP_CAL_CNT, CAP_READ_TIMEOUT);
	Serial.println("Capacitance calibrated");
  
  weight_setup();
  // Skip some of the first ones
  weight_recalibrate(WEIGHT_CAL_CNT, WEIGHT_READ_TIMEOUT);
  weight_recalibrate(WEIGHT_CAL_CNT, WEIGHT_READ_TIMEOUT);
  weight_recalibrate(WEIGHT_CAL_CNT, WEIGHT_READ_TIMEOUT);
	Serial.println("Weight calibrated");
  

  comms_setup();
	Serial.println("Communications set up");
  
	pwr_init();
	Serial.println("Power initialized");
	Serial.println("Chair Jordan is ready to go!");
  Serial.flush();
  delay(100);
  
}
// Main loop for the micro controller
// Remember to use Serial.flush() before sleep
void loop() {
  int cap = cap_read(1000);
  Serial.printf("Cap: %d\n", cap);
  int weight = weight_read(1000);
  Serial.printf("Weight: %d\n", weight);
  Serial.flush();
  delay(100);

  if (pwr_should_sleep(1)) {
    Serial.println("Start idling");
    Serial.flush();
    delay(100);
    pwr_idle(1);
    Serial.println("Stopped idling");
    Serial.flush();
    delay(100);
  }
}
