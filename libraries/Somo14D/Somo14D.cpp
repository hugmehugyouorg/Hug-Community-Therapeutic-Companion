#include "Somo14D.h"

Somo14D::Somo14D(uint8_t resetPin, uint8_t clockPin, uint8_t dataPin, uint8_t busyPin) :
	Audio(resetPin, clockPin, dataPin, busyPin)
{
}

void Somo14D::begin(void)
{
	digitalWrite(_clockPin, HIGH);
	digitalWrite(_dataPin, LOW);

	// reset the module
	digitalWrite(_resetPin, HIGH);
	delay(100);
	digitalWrite(_resetPin, LOW);
	delay(10);
	digitalWrite(_resetPin, HIGH);
	delay(100);

	Audio::begin();
}

void Somo14D::sendCommand(unsigned int command)
{
	// start bit
	digitalWrite(_clockPin, LOW);
	delay(2);

	// bit15, bit14, ... bit0
	for (unsigned int mask = 0x8000; mask > 0; mask >>= 1) {
		if (command & mask) {
			digitalWrite(_dataPin, HIGH);
		}
		else {
			digitalWrite(_dataPin, LOW);
		}
		// clock low
		digitalWrite(_clockPin, LOW);
		delayMicroseconds(200);

		// clock high
		digitalWrite(_clockPin, HIGH);
		delayMicroseconds(200);
	}

	// stop bit
	delay(2);
}