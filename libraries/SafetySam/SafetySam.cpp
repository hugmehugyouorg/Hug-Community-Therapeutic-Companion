#include <SafetySam.h>

SafetySam::SafetySam(SafetySamVoice *voice, Emotion *emotion, PlayMessages *playMessages, ServerProxy *proxy, Battery *battery, Stream *debug) :
	_voice(voice),
	_emotion(emotion),
	_playMessages(playMessages),
	_proxy(proxy),
	_battery(battery),
	_debug(debug)
{
	_readyToPlay = false;
}

void SafetySam::begin() {
	_voice->begin();
	_emotion->begin();
	_playMessages->begin();
	_proxy->begin();
}

void SafetySam::update() {
	uint8_t emotionState;
	boolean batteryUpdate = _battery->update();
	boolean emotionUpdate = _emotion->update();
	boolean playMessagesUpdate = _playMessages->update();
	uint8_t playMessagesState = _playMessages->getState();
	boolean proxyUpdate = _proxy->update();
	const char* message = _proxy->getIncoming();
	boolean saidUpdate = false;
	boolean messageSaidUpdate = false;
	uint16_t msg;
	uint8_t voltage[2];
	uint8_t updateFlags = 0;
	
	//there was data received from the proxy
	if(proxyUpdate) {
	
		//turn the string into a message we can use
		msg = _voice->strMsgToUintMsg(message);
		
		//store the message if it's playable
		if(msg != SafetySamVoice::ERROR)
			_playMessages->setMessage(msg);
		else //else this is not a proxy update like we need
			proxyUpdate = false;
		
		if(_debug != NULL) {
			if(proxyUpdate)
				_debug->print("Can say message: ");
			else
				_debug->print("Can't say message: ");
			_debug->println(message);
			_debug->print("message length: ");
			_debug->println(strlen(message));
			for(int i = 0; i < strlen(message); i++) {
				_debug->print(message[i]);
				_debug->print(", ");
				_debug->println(message[i], DEC);
			}
		}
		
		//now reset the proxy, we've got what we need
		_proxy->resetIncoming();
	}
	
	//emotion feedback needs to be immediate
	if( emotionUpdate ) {
		emotionState = _emotion->getState();
		switch( emotionState ) {
			case Emotion::HAPPY: _voice->happy(); saidUpdate = true; break;
			case Emotion::UNHAPPY: _voice->unhappy(); saidUpdate = true; break;
			case Emotion::EMERGENCY: _voice->emergency(); saidUpdate = true; break;
		}
	} //playMessagesState allows for the case where the internet is slow mainly...
	else if( playMessagesUpdate || playMessagesState ) {
		if(_playMessages->hasMessage()) {
			_voice->say( _playMessages->getMessage() );
			_playMessages->clearMessage();
			messageSaidUpdate = true;
		} //no messages, report energy level
		else if(playMessagesUpdate) {
			if(_battery->isLowBatteryAlert())
				_voice->batteryLow();
			else
				_voice->batteryCharged();
			saidUpdate = true; 
		}
	}
	else if( !_readyToPlay ) {
		_voice->readyToPlay();
		saidUpdate = true;
	}
	
	//specify what caused the update
	if(saidUpdate)
		updateFlags |= 32;
	if(messageSaidUpdate)
		updateFlags |= 16;
	if(batteryUpdate)
		updateFlags |= 8;
	if(emotionUpdate)
		updateFlags |= 4;
	if(playMessagesUpdate)
		updateFlags |= 2;
	if(!_readyToPlay)
		updateFlags |= 1;
	
	//there is an update if any submodule has an update or we are just now ready to play (on startup) or the watchdawg bit
	if( updateFlags > 0 || watchDawgBit() ) {
	
		_readyToPlay = true;
		
		if( !_proxy->willOverflowOutgoing( STATE_BITS + _battery->getVoltage0BitLength() + _battery->getVoltage1BitLength() + _battery->getChargingBitLength() + Emotion::STATE_BITS + PlayMessages::STATE_BITS + 2 * SafetySamVoice::STATE_BITS ) ) {
			
			if(_debug != NULL) {
				_debug->println("Update Flags (32 = said update, 16 = message said update, 8 = battery update, 4 = emotion update, 2 = play messages update, 1 = ready to play update... flags can be combined obviously)");
			}
			_proxy->setOutgoing(updateFlags, STATE_BITS);
			_battery->getVoltage(voltage);
			if(_debug != NULL) {
				_debug->println("Voltage Reading (whole number, e.g. #.00):");
			}
			_proxy->setOutgoing(voltage[0], _battery->getVoltage0BitLength());
			if(_debug != NULL) {
				_debug->println("Voltage (after decimal point, e.g. 0.##):");
			}
			_proxy->setOutgoing(voltage[1], _battery->getVoltage1BitLength());
			if(_debug != NULL) {
				_debug->println("Is Charging? (0 = NO, 1 = YES)");
			}
			_proxy->setOutgoing(_battery->getCharging(), _battery->getChargingBitLength());
			if(_debug != NULL) {
				_debug->println("Emotional State (0 - None, 1 - Happy, 2 - UNHAPPY, 3 - EMERGENCY)");
			}
			_proxy->setOutgoing(_emotion->getState(), Emotion::STATE_BITS);
			if(_debug != NULL) {
				_debug->println("Should Play Messages? (0 = NO, 1 = YES)");
			}
			_proxy->setOutgoing(playMessagesState, PlayMessages::STATE_BITS);
			if(_debug != NULL) {
				_debug->println("Last thing Safety Sam said:");
			}
			_proxy->setOutgoing(_voice->getLastSaid(), SafetySamVoice::STATE_BITS);
			if(_debug != NULL) {
				_debug->println("Last Safety Team message Safety Sam said.");
			}
			_proxy->setOutgoing(_voice->getLastMessageSaid(), SafetySamVoice::STATE_BITS);
			_proxy->endOutgoing();
		}
		
	}
}

boolean SafetySam::isProcessing() {
	return _battery->isProcessing() || _emotion->isProcessing() || _playMessages->isProcessing() || _proxy->isProcessing() || _voice->isProcessing();
}

boolean SafetySam::watchDawgBit() {
	return !_proxy->isWaitingToFlush() && (millis() - _proxy->getLastOutgoingSentTime()) > WATCHDAWG_SHOULD_BITE;
}

