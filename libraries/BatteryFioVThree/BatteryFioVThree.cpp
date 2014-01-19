#include <BatteryFioVThree.h>

BatteryFioVThree::BatteryFioVThree()
{
	_chargeState = NOT_CHARGING;
	_lowBatteryAlert = false;
}

boolean BatteryFioVThree::update() {
	uint8_t chargeState = getCharging();
	boolean rv = false;
	
	if(_chargeState != chargeState) {
		_chargeState = chargeState;
		rv = true;
	}
	
	if(chargeState)
		_lowBatteryAlert = false;
		
	if(!chargeState && !_lowBatteryAlert && getVoltage() < 3.5)
	{
		_lowBatteryAlert = true;
		rv = true;
	}
		
	return rv;
}

boolean BatteryFioVThree::isProcessing() {
	//if charging then processing
	if(_chargeState == CHARGING)
		return true;
		
	//otherwise we are not processing
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

//logic here is that the 3.7V lipo battery should not be able to reach over 4.2V
//so if that is the case we're going to assume we are not battery powered
uint8_t BatteryFioVThree::getCharging() {
	return (readVcc() > 4.2 || getVoltage() > 4.2 ? 1 : 0);
}

//SEE http://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/
double BatteryFioVThree::readVcc() {
  double Vcc = 5.0; // not necessarily true
int value = analogRead(0);
double volt = (value / 1023.0) * Vcc; // only correct if Vcc = 5.0 volts
return volt;
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
