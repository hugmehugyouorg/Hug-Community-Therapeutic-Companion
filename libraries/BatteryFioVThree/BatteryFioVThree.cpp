#include <BatteryFioVThree.h>

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
	int stat = digitalRead(2);
    return (stat == LOW) ? 1 : 0;
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