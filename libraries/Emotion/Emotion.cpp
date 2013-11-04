#include <Emotion.h>

Emotion::Emotion(Button2 *happyButton, Button2 *unhappyButton) :
	_happyButton(happyButton),
    _unhappyButton(unhappyButton)
{
}

void Emotion::begin() {
	_happyButton->begin();
	_unhappyButton->begin();
}

boolean Emotion::update() {
	uint8_t happyButtonUpdate;
	uint8_t unhappyButtonUpdate;
	boolean hasHappyStateUpdate;
	boolean hasUnhappyStateUpdate;
	boolean rv;
	
	// Happy & Unhappy Button States
	happyButtonUpdate = _happyButton->update();
	unhappyButtonUpdate = _unhappyButton->update();
	
	// Updates
	hasHappyStateUpdate = happyButtonUpdate == Button2::PRESS || happyButtonUpdate == Button2::RAPID_PRESS;
	hasUnhappyStateUpdate = unhappyButtonUpdate == Button2::PRESS || unhappyButtonUpdate == Button2::RAPID_PRESS;
	
	if( hasHappyStateUpdate || hasUnhappyStateUpdate ) {
		// Emotional State
		if(unhappyButtonUpdate == Button2::RAPID_PRESS)
			_state = EMERGENCY;
		else if(hasHappyStateUpdate)
			_state = HAPPY;
		else
			_state = UNHAPPY;
		rv = true;
	}
	else
		rv = false;
		
	return rv;
}

boolean Emotion::isProcessing() {
	return _happyButton->isProcessing() || _unhappyButton->isProcessing();
}

uint8_t Emotion::getState() {
	return _state;
}