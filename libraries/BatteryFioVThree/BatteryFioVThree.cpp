#include <BatteryFioVThree.h>

BatteryFioVThree::BatteryFioVThree()
{
	_chargeState = NOT_CHARGING;
	_lowBatteryAlert = false;
}

boolean BatteryFioVThree::update() {
	boolean rv = false;
	double voltage = getVoltage();
	//logic here is that the 3.7V lipo battery should not be able to reach over CHARGING_THRESHOLD volts
	//so if that is the case we're going to assume we are not battery powered
	_chargeState = (voltage > CHARGING_THRESHOLD ? 1 : 0);
	//logic here is that if the 3.7V lipo battery goes under LOW_BATTERY_THRESHOLD volts
	//then we're going to alert
	_lowBatteryAlert = (voltage < LOW_BATTERY_THRESHOLD ? true : false);
		
	return rv;
}

boolean BatteryFioVThree::isProcessing() {
	//never processing
	return false;
}

boolean BatteryFioVThree::isLowBatteryAlert() {
	return _lowBatteryAlert;
}

double BatteryFioVThree::getVoltage() {
	double sampleValue = analogRead(A11);
	double voltage = (sampleValue * (3.3/1024)) * 2;
	return voltage;
}

void BatteryFioVThree::getVoltage(uint8_t* voltage) {
	double v = getVoltage();
	
	//just get the whole number
	voltage[0] = (uint8_t)floor(v);
	
	//just get the decimal part, shift it by 2 places, then round to the nearest int
	voltage[1] = round((v - voltage[0]) * 100);
}

uint8_t BatteryFioVThree::getCharging() {
	return _chargeState;
}

uint8_t BatteryFioVThree::getVoltageBitLength() {
	return VOLTAGE_BITS;
}

uint8_t BatteryFioVThree::getVoltage0BitLength() {
	return VOLTAGE_BITS_ZERO;
}	

uint8_t BatteryFioVThree::getVoltage1BitLength() {
	return VOLTAGE_BITS_ONE;
}

uint8_t BatteryFioVThree::getChargingBitLength() {
	return CHARGING_BITS;
}