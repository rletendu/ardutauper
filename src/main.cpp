#include <Arduino.h>
#include <FastLED.h>
#include <IRremote.hpp>

#define LED_PIN 13
#define RELAY_PIN 12  
#define IR_RECEIVE_PIN 2
#define DECODE_NEC
#define SENSOR_PIN 3  

// How many leds in your strip?
#define NUM_LEDS 1

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 3

// Define the array of leds
CRGB leds[NUM_LEDS];

enum ardutauper_state {
  STATE_OFF,
  STATE_PRE_ARMED,
  STATE_ARMED,
  STATE_FIRED
};

void trigger(void);

void setup() {
  ardutauper_state current_state = STATE_OFF;
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  pinMode(SENSOR_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Ensure relay is off at start
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver}
  attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), trigger, CHANGE);
}

void loop() {
  if (IrReceiver.decode()) {
    // A code has been received
    IrReceiver.printIRResultShort(&Serial); // Print a short summary of received data
    IrReceiver.resume(); // Continue receiving
  }
  if (IrReceiver.available()) {
    IRData *receivedData = IrReceiver.read(); // Read the received data (pointer)
    if (receivedData != nullptr) {
      // Process receivedData as needed, e.g. access fields via receivedData->...
    }
  }
}

void trigger(void) {
  // This function can be used to trigger actions based on sensor input
}
