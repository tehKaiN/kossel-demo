#ifndef _KD_PINS_H
#define _KD_PINS_H

#define SAFETY_PIN 3

#define X_MAX_PIN 2
#define Y_MAX_PIN 15
#define Z_MAX_PIN 19

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_CS_PIN           53

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_CS_PIN           49

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_CS_PIN           40

extern uint8_t g_ubMaxX, g_ubMaxY, g_ubMaxZ, g_ubSafety;

#endif // _KD_PINS_H
