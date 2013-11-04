#ifndef SAFETY_SAM_VOICE_H
#define SAFETY_SAM_VOICE_H

#include <Arduino.h>
#include "../Audio/Audio.h"

class SafetySamVoice  {
	public:
		
		SafetySamVoice(Audio *player);
		void begin();
		void readyToPlay();
		void batteryLow();
		void batteryCharged();
		void quietTime();
		void happy();
		void unhappy();
		void emergency();
		boolean canSay(uint16_t message);
		boolean canSay(const char* message);
		boolean say(uint16_t message);
		boolean say(const char* message);
		uint16_t getLastSaid();
		uint16_t getLastMessageSaid();
		
		//THE NUMBER OF BITS IN THE STATE
		static const uint8_t STATE_BITS = 10;
		
	//------------------------------------------------------------------------------
	private:
	
		uint16_t randomPlay(uint16_t offset, uint16_t length);
	
		//SYSTEM EVENTS
		static const uint16_t READY_TO_PLAY_OFFSET = 0;
		static const uint16_t READY_TO_PLAY_LENGTH = 1;
		
		static const uint16_t BATTERY_LOW_OFFSET = 20;
		static const uint16_t BATTERY_LOW_LENGTH = 3;
		
		static const uint16_t BATTERY_CHARGED_OFFSET = 40;
		static const uint16_t BATTERY_CHARGED_LENGTH = 3;
		
		//QUIET TIME EVENTS
		static const uint16_t QUITE_TIME_BUTTON_CLICKED_OFFSET = 100;
		static const uint16_t QUITE_TIME_BUTTON_CLICKED_LENGTH = 3;
		
		//HAPPY EVENTS
		static const uint16_t HAPPY_BUTTON_CLICKED_OFFSET = 120;
		static const uint16_t HAPPY_BUTTON_CLICKED_LENGTH = 9;
		
		//UNHAPPY EVENTS
		static const uint16_t UNHAPPY_BUTTON_CLICKED_OFFSET = 140;
		static const uint16_t UNHAPPY_BUTTON_CLICKED_LENGTH = 8;
		
		//STANDARD MESSAGE EVENTS
		static const uint16_t STANDARD_MESSAGE_OFFSET = 200;
		static const uint16_t STANDARD_MESSAGE_LENGTH = 11;
		
		//SAFETY TIME EVENTS
		static const uint16_t UNHAPPY_BUTTON_RAPIDLY_CLICKED_OFFSET = 160;
		static const uint16_t UNHAPPY_BUTTON_RAPIDLY_CLICKED_LENGTH = 3;
		
		//SAFETY TIME MESSAGE EVENTS
		static const uint16_t SAFETY_TIME_MESSAGE_EVENTS_OFFSET = 300;
		static const uint16_t SAFETY_TIME_MESSAGE_EVENTS_LENGTH = 3;
		
		//LAST SAID
		static const int NONE = 0;
		
		Audio *_player;
		
		uint16_t _lastSaid;
		uint16_t _lastMessageSaid;
};

#endif // SAFETY_SAM_VOICE_H
