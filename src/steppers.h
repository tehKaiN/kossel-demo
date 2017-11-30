#ifndef _KD_STEPPERS_H
#define _KD_STEPPERS_H

#include <Arduino.h>
#include "pins.h"

struct tStepperPins {
	uint8_t ubStep;
	uint8_t ubDir;
	uint8_t ubEn;
	uint8_t ubCs;
};

#define STEPPER_X 1
#define STEPPER_Y 2
#define STEPPER_Z 4

constexpr tStepperPins g_pPins[3] = {
	{X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_CS_PIN},
	{Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_CS_PIN},
	{Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, Z_CS_PIN}
};

void stepperSetup(void);

void stepperStep(uint8_t ubMotorFlags);

void stepperStop(void);
void stepperStart(void);

/**
 * @return 1 on movement end, otherwise 0.
 */
uint8_t steppersZeroPosLoop(void);

void stepperDir(int8_t bDirX, int8_t bDirY, int8_t bDirZ);

extern uint16_t g_uwPosX, g_uwPosY, g_uwPosZ;
extern uint16_t g_uwDestPosX, g_uwDestPosY, g_uwDestPosZ;

#endif // _KD_STEPPERS_H
