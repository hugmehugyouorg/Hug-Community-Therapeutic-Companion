#ifndef QUIET_TIME_H
#define QUIET_TIME_H

#include "../Button2/Button2.h"

class QuietTime  {
	public:
		QuietTime(Button2 *quiteTimeButton);
		
		void begin();
		boolean update();
		boolean isProcessing();
		boolean isEnabled();
		void end();
		uint8_t getState();
		
		//THE NUMBER OF BITS IN THE STATE
		static const uint8_t STATE_BITS = 1;
		
	//------------------------------------------------------------------------------
	private:
		Button2 *_quiteTimeButton;
		
		boolean _state;
};

#endif // QUIET_TIME_H
