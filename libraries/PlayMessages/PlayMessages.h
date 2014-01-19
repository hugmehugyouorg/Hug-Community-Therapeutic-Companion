#ifndef PLAY_MESSAGES_H
#define PLAY_MESSAGES_H

#include "../Button2/Button2.h"
#include "../SafetySamVoice/SafetySamVoice.h"

class PlayMessages  {
	public:
		PlayMessages(Button2 *button);
		
		void begin();
		boolean update();
		boolean postUpdate();
		boolean isProcessing();
		void setMessage(uint16_t msg);
		boolean hasMessage();
		uint16_t getMessage();
		void clearMessage();
		uint8_t getState();
		
		//THE NUMBER OF BITS IN THE STATE
		static const uint8_t STATE_BITS = 1;
		
	//------------------------------------------------------------------------------
	private:
		Button2 *_button;
		
		boolean _state;
		boolean _postState;
		
		uint16_t _msg;
};

#endif // PLAY_MESSAGES_H
