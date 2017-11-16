#include "steppers.h"

static uint8_t s_ubStarted = 0;

void stepperSetup(void) {
	for(uint8_t i = 0; i != 3; ++i) {
		pinMode(g_pPins[i].ubDir, OUTPUT);
		pinMode(g_pPins[i].ubEn, OUTPUT);
		pinMode(g_pPins[i].ubCs, OUTPUT);
		pinMode(g_pPins[i].ubStep, OUTPUT);

		digitalWrite(g_pPins[i].ubDir, LOW); // Low means up, Hi means down
		digitalWrite(g_pPins[i].ubEn, LOW);
		digitalWrite(g_pPins[i].ubCs, HIGH);
	}
	s_ubStarted = 1;
}

void stepperStep(uint8_t ubMotorFlags) {
	if(ubMotorFlags & STEPPER_X)
		digitalWrite(g_pPins[0].ubStep, 1);
	if(ubMotorFlags & STEPPER_Y)
		digitalWrite(g_pPins[1].ubStep, 1);
	if(ubMotorFlags & STEPPER_Z)
		digitalWrite(g_pPins[2].ubStep, 1);
	delayMicroseconds(100);

	if(ubMotorFlags & STEPPER_X)
		digitalWrite(g_pPins[0].ubStep, 0);
	if(ubMotorFlags & STEPPER_Y)
		digitalWrite(g_pPins[1].ubStep, 0);
	if(ubMotorFlags & STEPPER_Z)
		digitalWrite(g_pPins[2].ubStep, 0);
	delayMicroseconds(100);
}

void stepperStop(void) {
	digitalWrite(g_pPins[0].ubEn, 1);
	digitalWrite(g_pPins[1].ubEn, 1);
	digitalWrite(g_pPins[2].ubEn, 1);
	s_ubStarted = 0;
}

void stepperStart(void) {
	if(!s_ubStarted) {
		digitalWrite(g_pPins[0].ubEn, 0);
		digitalWrite(g_pPins[1].ubEn, 0);
		digitalWrite(g_pPins[2].ubEn, 0);
	}
	s_ubStarted = 1;
}
