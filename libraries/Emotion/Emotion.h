#ifndef EMOTION_H
#define EMOTION_H

#include "../Button2/Button2.h"

class Emotion  {
	public:
		Emotion(Button2 *happyButton, Button2 *unhappyButton);
		
		void begin();
		boolean update();
		boolean isProcessing();
		uint8_t getState();
		uint8_t getHappyButtonState();
		uint8_t getUnhappyButtonState();
		
		// States
		static const uint8_t NONE = 0;
		static const uint8_t HAPPY = 1; 	//H
		static const uint8_t UNHAPPY = 2; 	//U
		static const uint8_t EMERGENCY = 3; //G
		
		//THE NUMBER OF BITS IN THE STATE
		static const uint8_t STATE_BITS = 2;
		
		// Updates
		static const uint8_t NO_UPDATE = 0;
		static const uint8_t UPDATE = 1;
		
	//------------------------------------------------------------------------------
	private:
		Button2 *_happyButton;
		Button2 *_unhappyButton;
		
		uint8_t _state;
};

#endif // EMOTION_H
