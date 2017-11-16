#include <Arduino.h>
#include "pins.h"
#include "steppers.h"

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
	Serial.print("dupa");
}

int lMaxX, lMaxY, lMaxZ, lSafety;
int s_steps = 0;

void serPrintf(const char *szFmt, ...) {
	char szBfr[50];
	va_list vArgs;
	va_start(vArgs, szFmt);
	vsprintf(szBfr, szFmt, vArgs);
	va_end(vArgs);
	Serial.print(szBfr);
}

void loop() {
	// put your main code here, to run repeatedly:

	lSafety = digitalRead(SAFETY_PIN);
	lMaxX = digitalRead(X_MAX_PIN);
	lMaxY = digitalRead(Y_MAX_PIN);
	lMaxZ = digitalRead(Z_MAX_PIN);

	// if(lSafety) {
	// 	stepperStop();
	// 	return;
	// }
	// stepperStart();

	while(Serial.available()) {
		int lRead = Serial.parseInt();
		if(lRead > 0)
			s_steps = lRead;
	}

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
