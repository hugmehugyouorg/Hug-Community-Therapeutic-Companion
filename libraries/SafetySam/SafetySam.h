#ifndef SAFETY_SAM_H
#define SAFETY_SAM_H

#include <Arduino.h>
#include "../SafetySamVoice/SafetySamVoice.h"
#include "../Emotion/Emotion.h"
#include "../PlayMessages/PlayMessages.h"
#include "../ServerProxy/ServerProxy.h"
#include "../Battery/Battery.h"

class SafetySam {
	public:
	
		SafetySam(SafetySamVoice *voice, Emotion *emotion, PlayMessages *playMessages, ServerProxy *proxy, Battery *battery, Stream *debug = NULL);
		void begin();
		void update();
		boolean isProcessing();
		
	//------------------------------------------------------------------------------
	private:
		SafetySamVoice *_voice;
		Emotion *_emotion;
		PlayMessages *_playMessages;
		ServerProxy *_proxy;
		Battery *_battery;
		Stream *_debug;
		boolean _readyToPlay;
		
		//THE NUMBER OF BITS IN THE STATE
		static const uint8_t STATE_BITS = 6;
};

#endif // SAFETY_SAM_ssH
