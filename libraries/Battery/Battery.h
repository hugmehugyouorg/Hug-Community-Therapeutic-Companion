#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>

class Battery  {
	public:
		virtual double getVoltage() = 0;  // Needs to be implemented by each subclass
		virtual void getVoltage(uint8_t* voltage) = 0;  // Needs to be implemented by each subclass
		virtual uint8_t getCharging() = 0;  // Needs to be implemented by each subclass
		virtual uint8_t getVoltageBitLength() = 0;  // Needs to be implemented by each subclass
		virtual uint8_t getVoltage0BitLength() = 0;  // Needs to be implemented by each subclass
		virtual uint8_t getVoltage1BitLength() = 0;  // Needs to be implemented by each subclass
		virtual uint8_t getChargingBitLength() = 0;  // Needs to be implemented by each subclass
};

#endif // BATTERY_H
