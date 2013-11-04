#include "Audio.h"

Audio::Audio(uint8_t resetPin, uint8_t clockPin, uint8_t dataPin, uint8_t busyPin) :
	_resetPin(resetPin),
	_clockPin(clockPin),
	_dataPin(dataPin),
	_busyPin(busyPin)
{
	
	pinMode(_clockPin, OUTPUT);
	pinMode(_dataPin, OUTPUT);
	pinMode(_resetPin, OUTPUT);
	if(_busyPin != 255)
		pinMode(_busyPin, INPUT);
}

void Audio::begin() {
	sendCommand(VOLUME_MAX);
}

boolean Audio::isPlaying(){  
  if(_busyPin == 255)
  	return false;
  return (digitalRead(_busyPin) == HIGH);
}

void Audio::play(unsigned int id)
{
	if(id > FILE_MAX)
		return;
		
	sendCommand(id);
}

void Audio::stop()
{
	sendCommand(STOP);
}

boolean Audio::canPlay(unsigned int id) {
	if(id > FILE_MAX)
		return false;
	return true;
}

boolean Audio::canPlay(const char* id) {
	return isUint16_t(id) && canPlay( toUint16_t(id) );
}

uint16_t Audio::toId(const char* id) {
	return toUint16_t(id);
}

uint16_t Audio::toUint16_t(const char* value) {
	return (uint16_t) strtoul(value, NULL, 0);
}

boolean Audio::isUint16_t(const char* value) {
	int firstNonZero = -1;
	int len = strlen(value);
	char toTest;
	int testingLen = 0;
	char testing[7];
	
	for(int i = 0; i < len; i++) {
		toTest = value[i];
		
		//if not a number then false
		if( toTest < '0' || toTest > '9') {
			return false;
		}
		
		if( firstNonZero == -1 && toTest != '0'  ) {
			//if greater than 5 possible digits then false
			if(len - i > 5)  {
				return false;
			}
			firstNonZero = i;
		}
		
		if(firstNonZero != -1)
			testing[testingLen++] = toTest;
	}
	
	//none found then false
	if(testingLen == 0)
		return false;
	
	testing[testingLen] = '\0';
	
	//if greater than the max value of an uint16_t then false
	if( strtoul((const char*)testing, NULL, 0) > 65535)
		return false;
		
	return true;
}
