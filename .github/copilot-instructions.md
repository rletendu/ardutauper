# Copilot Instructions for ardutauper

## Project Overview
Arduino-based triggering/firing system with IR remote control, LED indicators, relay control, and sensor input. Built for **ATmega328P (Arduino Nano)** using PlatformIO.

## Hardware Configuration
- **Target Board**: `nanoatmega328` (Arduino Nano with ATmega328P)
- **Pin Assignments** (defined in `src/main.cpp`):
  - LED_RED_PIN: 13 (status indicator)
  - LED_GREEN_PIN: 11 (status indicator)
  - RELAY_PIN: 12 (controls external relay)
  - IR_RECEIVE_PIN: 2 (IR remote receiver)
  - SENSOR_PIN: 3 (interrupt-driven trigger sensor)

## Architecture & State Machine
The system uses a **state machine** with 4 states (`ardutauper_state` enum):
- `STATE_OFF`: Idle, alternating LED pattern (red/green @ 1Hz)
- `STATE_PRE_ARMED`: Pre-arming phase (implementation pending)
- `STATE_ARMED`: System ready to fire (implementation pending)
- `STATE_FIRED`: Post-fire state (implementation pending)

State transitions are triggered by IR remote commands (currently detecting NEC protocol command `0xFFA25D`).

## Key Patterns & Conventions
- **Inline helper functions** for hardware control: Use `static inline` functions like `ledRedOn()`, `ledGreenOff()` for GPIO operations
- **Interrupt-driven sensor**: `SENSOR_PIN` (pin 3) uses `attachInterrupt()` with `CHANGE` mode calling `trigger()` function
- **Timing patterns**: Use `millis()` for non-blocking timing (see STATE_OFF case with 500ms LED toggle)
- **IR library**: IRremote v4.5.0 - call `IrReceiver.decode()` in loop, then `IrReceiver.printIRResultRawFormatted()` for debugging

## Build & Development Workflow
- **Build**: Use PlatformIO - `platformio run` or VS Code PlatformIO tasks
- **Upload**: `platformio run --target upload` (ensure correct COM port in platformio.ini if needed)
- **Monitor**: `platformio device monitor` (9600 baud - see `Serial.begin(9600)`)
- **Required Extension**: PlatformIO IDE (`platformio.platformio-ide`)

## Dependencies
- **IRremote** (z3t0/IRremote@^4.5.0): Manages IR receiver decoding
- Framework: Arduino (AVR platform)

## Critical Implementation Notes
1. **Relay Safety**: `RELAY_PIN` initialized LOW in `setup()` to ensure safe startup
2. **Variable Scope Issue**: `uint32_t time` declared inside `case STATE_OFF` - consider moving to broader scope if needed across states
3. **Incomplete State Handlers**: `STATE_PRE_ARMED`, `STATE_ARMED`, and `STATE_FIRED` cases are placeholders awaiting implementation
4. **IR Command Handling**: Currently only responds to `0xFFA25D` (POWER button) - expand command handling for state transitions
5. **Trigger Function**: `trigger()` interrupt handler is empty - implement sensor response logic here

## When Adding Features
- New hardware pins: Add `#define` at top with other pin definitions
- New states: Extend `ardutauper_state` enum and add case in main loop switch
- IR commands: Decode in `loop()` after `IrReceiver.decode()`, check `IrReceiver.decodedIRData.command`
- Timing operations: Always use `millis()` for non-blocking delays (never use `delay()` in interrupt handlers)
