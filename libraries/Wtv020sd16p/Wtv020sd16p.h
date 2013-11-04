#ifndef Wtv020sd16p_h
#define Wtv020sd16p_h

#include "../Audio/Audio.h"

class Wtv020sd16p : public Audio
{
	public:
		Wtv020sd16p(uint8_t resetPin, uint8_t clockPin, uint8_t dataPin, uint8_t busyPin = 255);
		virtual void begin();
  
	protected:
		virtual void sendCommand(unsigned int command);
};

#endif /* Wtv020sd16p_h */


