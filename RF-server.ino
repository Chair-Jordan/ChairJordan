/*
 * This file contains the logic for the server
 * It is meant to run on an esp32 using a RFM69HW radio module
 */

#include <RFM69.h>
#include <SPI.h>

// Addresses for this node
#define NETWORKID     0   // Must be the same for all connected nodes
#define MYNODEID      1   // ID for the server is 1

// RFM69 frequency
#define FREQUENCY RF69_433MHZ

// AES encryption
#define ENCRYPT       false
#define ENCRYPTKEY    "TOPSECRETPASSWRD" // Use the same 16-byte key on all nodes

// Requests ACK
#define USEACK true

// Pins
#define PIN_LED 8
#define PIN_SLAVESELECT 7
#define PIN_INTERUPT 2

// Create a library object for RFM69HCW
RFM69 radio(PIN_SLAVESELECT, PIN_INTERUPT, true);

void setup()
{
  Serial.begin(9600);
  Serial.println(F("Node number "));
  Serial.println(MYNODEID,DEC);

  Serial.print(F("SCK: "));
  Serial.println(SCK);
  Serial.print(F("MOSI: "));
  Serial.println(MOSI);
  Serial.print(F("MISO: "));
  Serial.println(MISO);
  Serial.print(F("SS: "));
  Serial.println(SS);

  // Set up the indicator LED (optional):
  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED,LOW);

  // Initialize the RFM69HCW:
  radio.initialize(FREQUENCY, MYNODEID, NETWORKID);
  radio.setHighPower(); // Always use this for RFM69HCW

  // Turn on encryption if desired:
  if (ENCRYPT)
    radio.encrypt(ENCRYPTKEY);

  Serial.print(F("Frequency: "));
  Serial.println(radio.getFrequency());
}

void loop()
{
  // Receive data
  if (radio.receiveDone())
  {
    // Print out information
    Serial.print(F("Received from node "));
    Serial.println(radio.SENDERID, DEC);
 
    for (byte i = 0; i < radio.DATALEN; i++)
      Serial.print((char)radio.DATA[i]);

    // RSSI
    Serial.print(F("], RSSI "));
    Serial.println(radio.RSSI);

    // Send an ACK if requested
    if (radio.ACKRequested())
    {
      radio.sendACK();
      Serial.println(F("ACK sent"));
    }
    Blink(PIN_LED,10);
  }
}

// Blink an LED for a given duration
void Blink(byte PIN, int DELAY_MS)
{
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
  delay(DELAY_MS);
}
