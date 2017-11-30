#include <Arduino.h>
#include "pins.h"
#include "steppers.h"

void serPrintf(const char *szFmt, ...) {
	char szBfr[50];
	va_list vArgs;
	va_start(vArgs, szFmt);
	vsprintf(szBfr, szFmt, vArgs);
	va_end(vArgs);
	Serial.print(szBfr);
}

void setup() {
	// put your setup code here, to run once:

	Serial.begin(9600);
	pinMode(X_MAX_PIN, INPUT);
	digitalWrite(X_MAX_PIN, HIGH);

	pinMode(Y_MAX_PIN, INPUT);
	digitalWrite(Y_MAX_PIN, HIGH);

	pinMode(Z_MAX_PIN, INPUT);
	digitalWrite(Z_MAX_PIN, HIGH);

	pinMode(SAFETY_PIN, INPUT);
	digitalWrite(SAFETY_PIN, HIGH);

	stepperSetup();
	Serial.print("HELO");
}

int lMaxX, lMaxY, lMaxZ, lSafety;
int s_steps = 0;

char s_szBfr[30];
uint8_t s_ubBfrLen = 0;

uint8_t cmdInterpret(void) {
	int16_t uwTmpX, uwTmpY, uwTmpZ;

	// No data - skip
	if(!s_ubBfrLen)
		return 0;

	// Got correct data?
	if(sscanf(s_szBfr, "x %d y %d z %d", &uwTmpX, &uwTmpY, &uwTmpZ) == 3) {
		serPrintf("x: %d, y: %d, z: %d\r\n", uwTmpX, uwTmpY, uwTmpZ);
		return 1;
	}

	// Got wrong data
	Serial.write("ERR(parse)\r\n");
	return 0;
}

void loop() {
	// put your main code here, to run repeatedly:

	lSafety = digitalRead(SAFETY_PIN);
	lMaxX = digitalRead(X_MAX_PIN);
	lMaxY = digitalRead(Y_MAX_PIN);
	lMaxZ = digitalRead(Z_MAX_PIN);

	while(Serial.available()) {
		s_szBfr[s_ubBfrLen++] = Serial.read();
		if(s_szBfr[s_ubBfrLen-1] == '\r' || s_szBfr[s_ubBfrLen-1] == '\n') {
			s_szBfr[s_ubBfrLen-1] = 0; // Make string null-terminated
			cmdInterpret();
			s_ubBfrLen = 0;
		}
		s_steps = 0;
	}

	if(lSafety) {
		stepperStop();
		return;
	}
	stepperStart();

	if(s_steps > 0) {
		uint8_t ubMotors = 0;
		if(!lMaxX)
			ubMotors |= STEPPER_X;
		if(!lMaxY)
			ubMotors |= STEPPER_Y;
		if(!lMaxZ)
			ubMotors |= STEPPER_Z;
		stepperStep(ubMotors);
		--s_steps;
	}

	// serPrintf("pin state: %d %d %d\r\n", lMaxX, lMaxY, lMaxZ);
}
