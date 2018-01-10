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

/**
 * @brief Initializes steppers' state.
 */
void stepperSetup(void);

/**
 * @brief Steps selected motors.
 *
 * @param ubMotorFlags: OR combination of STEPPER_X, STEPPER_Y and STEPPER_Z.
 */
void stepperStep(uint8_t ubMotorFlags);

/**
 * @brief Stops stepper motors, making their rotors loose.
 */
void stepperStop(void);

/**
 * @brief Powers on stepper motors, making their rotors stiff.
 */
void stepperStart(void);

/**
 * @brief Zeroes stepper motors position.
 * Allows making sure that printer is in known position and stepper counters
 * are correct. After zeroing position, motors are moved 1000 steps away
 * from end.
 *
 * @return uint8_t 1 if zeroing is done, otherwise 0.
 */
uint8_t steppersZeroPosLoop(void);

/**
 * @brief Sets stepper motors' direction. 1 is to bottom, -1 is to top.
 *
 * @param bDirX: First stepper direction.
 * @param bDirY: Second stepper direction.
 * @param bDirZ: Third stepper direction.
 */
void stepperDir(int8_t bDirX, int8_t bDirY, int8_t bDirZ);

extern uint16_t g_uwPosX, g_uwPosY, g_uwPosZ; ///< Motors' positions in steps.
extern uint16_t g_uwDestPosX, g_uwDestPosY, g_uwDestPosZ; ///< Destination positions.

#endif // _KD_STEPPERS_H
