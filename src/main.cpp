#include <Arduino.h>
#include <IRremote.hpp>
#include <LowPower.h>

#include "hardware.h"

ardutauper_state current_state = STATE_OFF;
static bool triggered = false;

void trigger(void);

void setup() {
  
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT_PULLUP);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  relayOff();
  buzzerOff();
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver}
  attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), trigger, CHANGE);
  ledGreenOff();
  ledRedOff();
}

void loop() {
  
  static uint32_t rxtime=0;
  static uint32_t last_rxtime=0;
  static uint8_t rxcount=0;
  static uint32_t rx_buff[RX_BUFFER_SIZE];
  static uint32_t armed_time=0;

  if (IrReceiver.decode()) {
    IrReceiver.printIRResultRawFormatted(&Serial, true);
    rxtime=millis();
    if (rxtime-last_rxtime<1000) {
      rxcount++;
    } else {
      rxcount=1;
    }
    last_rxtime = rxtime;
    rx_buff[rxcount-1] = IrReceiver.decodedIRData.command;
    if (IrReceiver.decodedIRData.command == 0xFFA25D) { // Example NEC code for "POWER" button
      Serial.println("POWER button pressed");
      
    }
  }

  switch (current_state)
  {
  case STATE_OFF:
    uint32_t time=millis();
    if (time%1000<100) {
      ledGreenOn();
    } else {
      ledGreenOff();
    }
    if (rxcount>=3) {
      // Check if the last 3 received codes match a predefined sequence
      if (rx_buff[rxcount-1] == 0xFFA25D && rx_buff[rxcount-2] == 0xFFE21D && rx_buff[rxcount-3] == 0xFF629D) {
        Serial.println("Arming sequence detected");
        current_state = STATE_PRE_ARMED;
        rxcount=0; // Reset count after arming
        armed_time=millis();
      }
    }  
    break;
  
  case STATE_PRE_ARMED:
    if (time%1000<500) {
      buzzerOn();
      ledGreenOn();
      ledRedOff();
    } else {
      buzzerOff();
      ledGreenOff();
      ledRedOn();
    }  
    if (millis()-armed_time>20000) { // 20 seconds to confirm arming
      Serial.println("System armed");
      buzzerOn();
      delay(5000);
      buzzerOff();
      current_state = STATE_ARMED;
    } 
    if (rxcount) {
      Serial.println("Arming sequence interrupted");
      current_state = STATE_OFF;
      rxcount=0;
    }
    break;

  case STATE_ARMED:
    if (time%500<250) {
      ledRedOn();
    } else {
      ledRedOff();
    }
    if (triggered) {
      Serial.println("Trigger detected! Firing!");
      relayOn(); // Activate relay
      current_state = STATE_FIRED;
    }
    if (rxcount) {
      Serial.println("Disarming sequence detected");
      current_state = STATE_OFF;
      rxcount=0;
    }  
    break;

  case STATE_FIRED:
 
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    for(int i=0; i<5; i++) {
      ledRedToggle();
      ledGreenToggle();
      delay(200);
    } 
    
    ledGreenOff();
    ledRedOff();
    break;

  default:
  
    break;
  }
  
}

void trigger(void) {
  if (triggered) return; // Avoid multiple triggers
  triggered = true;
}
