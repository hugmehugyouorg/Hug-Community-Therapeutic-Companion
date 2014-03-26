#ifndef BATTERY_FIO_V_THREE_H
#define BATTERY_FIO_V_THREE_H

#include "../Battery/Battery.h"

class BatteryFioVThree : public Battery {
	public:
		virtual boolean update();
		virtual boolean isProcessing();
		virtual boolean isLowBatteryAlert();
		virtual double getVoltage();
		virtual void getVoltage(uint8_t* voltage);
		virtual uint8_t getCharging();
		virtual uint8_t getVoltageBitLength();
		virtual uint8_t getVoltage0BitLength();
		virtual uint8_t getVoltage1BitLength();
		virtual uint8_t getChargingBitLength();
		
		BatteryFioVThree();
		double readVcc();
		
	private:
		
		//states
		static const uint8_t NOT_CHARGING = 0;
		static const uint8_t CHARGING = 1;
		uint8_t _chargeState;
		boolean _lowBatteryAlert;
		
		//THE LOW BATTERY THRESHOLD
		static const double LOW_BATTERY = 3.5;
		
		//THE NUMBER OF BITS IN THE VOLTAGE
		static const uint8_t VOLTAGE_BITS = 32;
		
		//THE NUMBER OF BITS IN THE VOLTAGE as uint, first part (5V - 101)
		static const uint8_t VOLTAGE_BITS_ZERO = 3;
		
		//THE NUMBER OF BITS IN THE VOLTAGE as uint, second part (99 - 1100011)
		static const uint8_t VOLTAGE_BITS_ONE = 7;
		
		//THE NUMBER OF BITS IN CHARGING
		static const uint8_t CHARGING_BITS = 1;
};

#endif // BATTERY_FIO_V_THREE_H
