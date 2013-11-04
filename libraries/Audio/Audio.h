#ifndef AUDIO_H
#define AUDIO_H

#include <Arduino.h>

class Audio  {
	public:
		Audio(uint8_t clockPin, uint8_t dataPin, uint8_t busyPin = 255, uint8_t resetPin = 255);
		virtual void begin();  // Needs to be implemented by each subclass
		boolean isPlaying();
		void play(unsigned int id);
		void stop();
		boolean canPlay(unsigned int id);
		boolean canPlay(const char* id);
		uint16_t toId(const char* id);
	  
		static const unsigned int VOLUME_MAX = 0xFFF7;
		static const unsigned int STOP = 0xFFFF;
		static const unsigned int FILE_MAX = 0x01FF;
		
	protected:
		virtual void sendCommand(unsigned int command) = 0;  // Needs to be implemented by each subclass
		uint16_t toUint16_t(const char* value);
		boolean isUint16_t(const char* value);
		uint8_t _resetPin;
		uint8_t _clockPin;
		uint8_t _dataPin;
		uint8_t _busyPin;
};

#endif // AUDIO_H
