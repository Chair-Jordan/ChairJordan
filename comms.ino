#include "comms.h"

RFM69 radio(7, 8, true);

void comms_setup(void) {
	radio.initialize(RF_FREQUENCY, RF_NODE, RF_NETWORK);
	radio.setHighPower();
	Serial.println(radio.getFrequency());
}
