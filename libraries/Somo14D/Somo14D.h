#ifndef SOMO14D_h
#define SOMO14D_h

#include "../Audio/Audio.h"

class Somo14D : public Audio
{

	public:
		Somo14D(uint8_t clockPin, uint8_t dataPin, uint8_t busyPin = 255, uint8_t resetPin = 255);
		virtual void begin();
  
	protected:
		virtual void sendCommand(unsigned int command);
};

#endif /* SOMO14D_h */

