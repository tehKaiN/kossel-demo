#include "steppers.h"

static uint8_t s_ubStarted = 0;
static int8_t s_pDirs[3] = {0, 0, 0};

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
	if((ubMotorFlags & STEPPER_X) && s_pDirs[0]) {
		if(s_pDirs[0] > 0 || g_uwPosX) {
			digitalWrite(g_pPins[0].ubStep, 1);
			g_uwPosX += s_pDirs[0];
		}
	}
	if((ubMotorFlags & STEPPER_Y) && s_pDirs[1]) {
		if(s_pDirs[1] > 0 || g_uwPosY) {
			digitalWrite(g_pPins[1].ubStep, 1);
			g_uwPosY += s_pDirs[1];
		}
	}
	if((ubMotorFlags & STEPPER_Z) && s_pDirs[2]) {
		if(s_pDirs[2] > 0 || g_uwPosZ) {
			digitalWrite(g_pPins[2].ubStep, 1);
			g_uwPosZ += s_pDirs[2];
		}
	}
	delayMicroseconds(100);

	if(ubMotorFlags & STEPPER_X)
		digitalWrite(g_pPins[0].ubStep, 0);
	if(ubMotorFlags & STEPPER_Y)
		digitalWrite(g_pPins[1].ubStep, 0);
	if(ubMotorFlags & STEPPER_Z)
		digitalWrite(g_pPins[2].ubStep, 0);
	delayMicroseconds(100);
}

void stepperDir(int8_t bDirX, int8_t bDirY, int8_t bDirZ) {
	digitalWrite(g_pPins[0].ubDir, (bDirX < 0 ? LOW: HIGH));
	digitalWrite(g_pPins[1].ubDir, (bDirY < 0 ? LOW: HIGH));
	digitalWrite(g_pPins[2].ubDir, (bDirZ < 0 ? LOW: HIGH));
	s_pDirs[0] = bDirX;
	s_pDirs[1] = bDirY;
	s_pDirs[2] = bDirZ;
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

uint8_t steppersZeroPosLoop(void) {
	uint8_t ubStepperFlags = 0;
	if(!g_ubMaxX)
		ubStepperFlags |= STEPPER_X;
	if(!g_ubMaxY)
		ubStepperFlags |= STEPPER_Y;
	if(!g_ubMaxZ)
		ubStepperFlags |= STEPPER_Z;
	stepperDir(-1, -1, -1);
	stepperStep(ubStepperFlags);

	if(!ubStepperFlags) {
		g_uwPosX = 0;
		g_uwPosY = 0;
		g_uwPosZ = 0;
		g_uwDestPosX = 1000;
		g_uwDestPosY = 1000;
		g_uwDestPosZ = 1000;
		return 1;
	}
	return 0;
}
