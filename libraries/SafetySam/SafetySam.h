#ifndef SAFETY_SAM_H
#define SAFETY_SAM_H

#include <Arduino.h>
#include "../SafetySamVoice/SafetySamVoice.h"
#include "../Emotion/Emotion.h"
#include "../QuietTime/QuietTime.h"
#include "../ServerProxy/ServerProxy.h"
#include "../Battery/Battery.h"

class SafetySam {
	public:
	
		SafetySam(SafetySamVoice *voice, Emotion *emotion, QuietTime *quiteTime, ServerProxy *proxy, Battery *battery, Stream *debug);
		void begin();
		void update();
		boolean isProcessing();
		
	//------------------------------------------------------------------------------
	private:
		SafetySamVoice *_voice;
		Emotion *_emotion;
		QuietTime *_quiteTime;
		ServerProxy *_proxy;
		Battery *_battery;
		Stream *_debug;
		boolean _readyToPlay;
};

#endif // SAFETY_SAM_H
