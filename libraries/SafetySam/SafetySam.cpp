#include <SafetySam.h>

SafetySam::SafetySam(SafetySamVoice *voice, Emotion *emotion, QuietTime *quiteTime, ServerProxy *proxy, Battery *battery, Stream *debug) :
	_voice(voice),
	_emotion(emotion),
	_quiteTime(quiteTime),
	_proxy(proxy),
	_battery(battery),
	_debug(debug)
{
	_readyToPlay = false;
}

void SafetySam::begin() {
	_voice->begin();
	_emotion->begin();
	_quiteTime->begin();
	_proxy->begin();
}

void SafetySam::update() {
	uint8_t emotionState;
	boolean emotionUpdate = _emotion->update();
	boolean quietTimeUpdate = _quiteTime->update();
	boolean beQuiet = _quiteTime->isEnabled();
	boolean proxyUpdate = _proxy->update();
	const char* message = _proxy->getIncoming();
	boolean hasUpdate;
	uint8_t voltage[2];
	
	//if the message can't be played on a server proxy update then
	//set proxy update to false and reset incoming
	if(proxyUpdate) {
		proxyUpdate = _voice->canSay(message);
		if(!proxyUpdate) {
			_proxy->resetIncoming();
		}
	}
	
	hasUpdate = emotionUpdate || quietTimeUpdate || proxyUpdate || !_readyToPlay;
	
	if( proxyUpdate && !beQuiet ) {
		_voice->say( message );
		_proxy->resetIncoming();
	}
	else if( emotionUpdate ) {
		emotionState = _emotion->getState();
		switch( emotionState ) {
			case Emotion::HAPPY: _quiteTime->end(); _voice->happy(); break;
			case Emotion::UNHAPPY: _quiteTime->end(); _voice->unhappy(); break;
			case Emotion::EMERGENCY: _quiteTime->end(); _voice->emergency(); break;
		}
	} 
	else if( quietTimeUpdate ) {
		_voice->quietTime();
	}
	else if( !_readyToPlay ) {
		_voice->readyToPlay();
	}
	
	if( hasUpdate ) {
		_readyToPlay = true;
		if( !_proxy->willOverflowOutgoing( Emotion::STATE_BITS + 1 + 2 * SafetySamVoice::STATE_BITS ) ) {
			_proxy->startOutgoing();
			_battery->getVoltage(voltage);
			if(_debug) {
				_debug->println("Voltage Reading (whole number, e.g. #.00):");
			}
			_proxy->setOutgoing(voltage[0], _battery->getVoltage0BitLength());
			if(_debug) {
				_debug->println("Voltage (after decimal point, e.g. 0.##):");
			}
			_proxy->setOutgoing(voltage[1], _battery->getVoltage1BitLength());
			if(_debug) {
				_debug->println("Is Charging? (0 = NO, 1 = YES)");
			}
			_proxy->setOutgoing(_battery->getCharging(), _battery->getChargingBitLength());
			if(_debug) {
				_debug->println("Emotional State (0 - Happy, 1 - UNHAPPY, 2 - EMERGENCY)");
			}
			_proxy->setOutgoing(_emotion->getState(), Emotion::STATE_BITS);
			if(_debug) {
				_debug->println("Is Quiet Time? (0 = NO, 1 = YES)");
			}
			_proxy->setOutgoing(_quiteTime->getState(), QuietTime::STATE_BITS);
			if(_debug) {
				_debug->println("Last thing Safety Sam said:");
			}
			_proxy->setOutgoing(_voice->getLastSaid(), SafetySamVoice::STATE_BITS);
			if(_debug) {
				_debug->println("Last Safety Team message Safety Sam said.");
			}
			_proxy->setOutgoing(_voice->getLastMessageSaid(), SafetySamVoice::STATE_BITS);
			_proxy->endOutgoing();
		}
	}
}

boolean SafetySam::isProcessing() {
	return _emotion->isProcessing() || _quiteTime->isProcessing() || _proxy->isProcessing();
}

