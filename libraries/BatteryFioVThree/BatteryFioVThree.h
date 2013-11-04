#ifndef BATTERY_FIO_V_THREE_H
#define BATTERY_FIO_V_THREE_H

#include "../Battery/Battery.h"

class BatteryFioVThree : public Battery {
	public:
		virtual double getVoltage();
		virtual void getVoltage(uint8_t* voltage);
		virtual uint8_t getCharging();
		virtual uint8_t getVoltageBitLength();
		virtual uint8_t getVoltage0BitLength();
		virtual uint8_t getVoltage1BitLength();
		virtual uint8_t getChargingBitLength();
		
	private:
		//THE NUMBER OF BITS IN THE VOLTAGE
		static const uint8_t VOLTAGE_BITS = 32;
		
		//THE NUMBER OF BITS IN THE VOLTAGE as uint, first part (5V - 1001)
		static const uint8_t VOLTAGE_BITS_ZERO = 4;
		
		//THE NUMBER OF BITS IN THE VOLTAGE as uint, second part (99 - 1100011)
		static const uint8_t VOLTAGE_BITS_ONE = 7;
		
		//THE NUMBER OF BITS IN CHARGING
		static const uint8_t CHARGING_BITS = 1;
};

#endif // BATTERY_FIO_V_THREE_H
