#ifndef HARDWARE_H
#define HARDWARE_H

// Pin definitions
#define LED_RED_PIN 13
#define LED_GREEN_PIN 11
#define RELAY_PIN 12  
#define IR_RECEIVE_PIN 2
#define BUZZER_PIN 4
#define SENSOR_PIN 3


// State machine enumeration
enum ardutauper_state {
  STATE_OFF,
  STATE_PRE_ARMED,
  STATE_ARMED,
  STATE_FIRED
};

// Inline hardware control functions
static inline void ledRedOn() {
  digitalWrite(LED_RED_PIN, HIGH);
}

static inline void ledRedOff() {
  digitalWrite(LED_RED_PIN, LOW);
}

static inline void ledRedToggle() {
  digitalWrite(LED_RED_PIN, !digitalRead(LED_RED_PIN));
}

static inline void ledGreenOn() {
  digitalWrite(LED_GREEN_PIN, HIGH);
}

static inline void ledGreenOff() {
  digitalWrite(LED_GREEN_PIN, LOW);
}

static inline void ledGreenToggle() {
  digitalWrite(LED_GREEN_PIN, !digitalRead(LED_GREEN_PIN));
}

static inline void relayOn() {
  digitalWrite(RELAY_PIN, HIGH);
}

static inline void relayOff() {
  digitalWrite(RELAY_PIN, LOW);
}

static inline void buzzerOn() {
  digitalWrite(BUZZER_PIN, HIGH);
}

static inline void buzzerOff() {
  digitalWrite(BUZZER_PIN, LOW);
}

#endif // HARDWARE_H
