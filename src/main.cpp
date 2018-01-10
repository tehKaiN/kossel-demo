#include <Arduino.h>
#include "pins.h"
#include "steppers.h"

uint16_t g_uwPosX = 65535, g_uwPosY = 65535, g_uwPosZ = 65535;
uint16_t g_uwDestPosX = 0, g_uwDestPosY = 0, g_uwDestPosZ = 0;
uint8_t s_ubDoCalib = 1;

#define SGN(x) (!(x) ? 0 : ((x) > 0 ? 1 : -1))

/**
 * @brief Printf-compatible function for serial port.
 *
 * @param szFmt: Printf-compatible format string.
 * @param ...: Printf-compatible format string args.
 */
void serPrintf(const char *szFmt, ...) {
	char szBfr[50];
	va_list vArgs;
	va_start(vArgs, szFmt);
	vsprintf(szBfr, szFmt, vArgs);
	va_end(vArgs);
	Serial.print(szBfr);
}

void setup() {
	Serial.begin(57600);
	pinMode(X_MAX_PIN, INPUT);
	digitalWrite(X_MAX_PIN, HIGH);

	pinMode(Y_MAX_PIN, INPUT);
	digitalWrite(Y_MAX_PIN, HIGH);

	pinMode(Z_MAX_PIN, INPUT);
	digitalWrite(Z_MAX_PIN, HIGH);

	pinMode(SAFETY_PIN, INPUT);
	digitalWrite(SAFETY_PIN, HIGH);

	stepperSetup();
	Serial.print("HELO\r\n");
}

uint8_t g_ubMaxX, g_ubMaxY, g_ubMaxZ; ///< Switches at top of movement range.
uint8_t g_ubSafety; ///< Safety switch.

char s_szBfr[30]; ///< Serial receive buffer.
uint8_t s_ubBfrLen = 0; ///< Receive data length.

/**
 * @brief Parses command residing in recv buffer info steppers' destination.
 *
 * @return 1 on success, otherwise 0.
 */
uint8_t cmdInterpret(void) {
	uint16_t uwTmpX, uwTmpY, uwTmpZ;

	// No data - skip
	if(s_ubBfrLen <= 1)
		return 0;

	if(sscanf(s_szBfr, "x %u y %u z %u", &uwTmpX, &uwTmpY, &uwTmpZ) == 3) {
		// Got correct data
		g_uwDestPosX = uwTmpX;
		g_uwDestPosY = uwTmpY;
		g_uwDestPosZ = uwTmpZ;
		return 1;
	}

	// Got wrong data
	Serial.write("ERR(parse)\r\n");
	return 0;
}

/**
 * @brief Reads current state of edge/safety pins.
 */
static void readPins(void) {
	g_ubSafety = digitalRead(SAFETY_PIN);
	g_ubMaxX = digitalRead(X_MAX_PIN);
	g_ubMaxY = digitalRead(Y_MAX_PIN);
	g_ubMaxZ = digitalRead(Z_MAX_PIN);
}

/**
 * @brief Receives data via serial link.
 * If complete line has been read, parses it to destination pos.
 */
static void serialRecv(void) {
	while(Serial.available()) {
		s_szBfr[s_ubBfrLen++] = Serial.read();
		if(s_szBfr[s_ubBfrLen-1] == '\r' || s_szBfr[s_ubBfrLen-1] == '\n') {
			s_szBfr[s_ubBfrLen-1] = 0; // Make string null-terminated
			cmdInterpret();
			s_ubBfrLen = 0;
		}
	}
}

void loop() {
	readPins();
	serialRecv();

	if(g_ubSafety) {
		// Safety switch not pressed - stop everything
		stepperStop();
		return;
	}
	stepperStart();

	if(s_ubDoCalib) {
		// Calibration was not done - do it now
		s_ubDoCalib = !steppersZeroPosLoop();
	}
	else {
		// Move to current destination pos
		stepperDir(
			SGN((int)g_uwDestPosX - (int)g_uwPosX),
			SGN((int)g_uwDestPosY - (int)g_uwPosY),
			SGN((int)g_uwDestPosZ - (int)g_uwPosZ)
		);
		uint8_t ubSteppers = 0;
		if(g_uwPosX != g_uwDestPosX)
			ubSteppers |= STEPPER_X;
		if(g_uwPosY != g_uwDestPosY)
			ubSteppers |= STEPPER_Y;
		if(g_uwPosZ != g_uwDestPosZ)
			ubSteppers |= STEPPER_Z;
		stepperStep(ubSteppers);
	}
}
