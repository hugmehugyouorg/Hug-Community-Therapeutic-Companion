#ifndef BUTTON2_H
#define BUTTON2_H

#include <Arduino.h>

class Button2  {
	public:
		Button2(uint8_t buttonPin, uint8_t groundPin = 255);
		void begin();
		uint8_t update();
		boolean isProcessing();
		
		// Configuration
		static const uint8_t PRESS_DEBOUNCE = 10;
		static const uint8_t RAPID_PRESSES = 3;
		static const uint16_t RAPID_PRESSES_WINDOW = 1000;
		static const uint16_t RAPID_PRESSES_DELAY = 500;
		
		// States
		static const uint8_t NO_CONTACT_DETECTED = 0;
		static const uint8_t CONTACT_DETECTED = 1;
		static const uint8_t PRESSING = 2;
		static const uint8_t PRESS = 3;
		static const uint8_t RAPID_PRESS = 4;
	//------------------------------------------------------------------------------
	private:
		unsigned long timeElapsedFirstPress();
		unsigned long timeElapsedLastPress();
	
		/** Pin connected to the pushbutton (active LOW) */
		const uint8_t _buttonPin;
		
		/** Pin used as ground if necessary */
		const uint8_t _groundPin;
		
		//current state
		uint8_t _state;
		
		/** Timestamp used to enter push state */
		unsigned long _pressDeadline;
		
		/** whether to track presses to determine PRESS or RAPID_PRESS behavior **/
		boolean _trackPresses;
		
		/** number of presses **/
		uint8_t _presses;
		
		/** milliseconds when the first press happend **/
		unsigned long _firstPress;
		
		/** milliseconds when the last pressing happend **/
		unsigned long _lastPress;
};

#endif // BUTTON2_H
