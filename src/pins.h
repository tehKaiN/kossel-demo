#ifndef _KD_PINS_H
#define _KD_PINS_H

/// Safety switch - if not pressed, printer won't move.
#define SAFETY_PIN 3

/// Safety switches - pressed by steppers if they reach top movement limit.
#define X_MAX_PIN 2
#define Y_MAX_PIN 15
#define Z_MAX_PIN 19

/// Pins for steering first stepper.
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_CS_PIN           53

/// Pins for steering second stepper.
#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_CS_PIN           49

/// Pins for steering third stepper.
#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_CS_PIN           40

extern uint8_t g_ubMaxX, g_ubMaxY, g_ubMaxZ; ///< Curr state of limit switches.
extern uint8_t g_ubSafety; ///< Current state of safety switch.

#endif // _KD_PINS_H
