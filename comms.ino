/*
 * Logic for communcation from node (chair) to server
 * The communication unit is RFM69HW
 * Library used: https://github.com/LowPowerLab/RFM69
*/

#include "comms.h"

RFM69 radio(7, 2, true);

void comms_setup(void) {
	radio.initialize(RF_FREQUENCY, RF_NODE, RF_NETWORK);
	radio.setHighPower();
	Serial.println(radio.getFrequency());
}
