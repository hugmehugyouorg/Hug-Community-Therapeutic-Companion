#include <PlayMessages.h>

PlayMessages::PlayMessages(Button2 *button) :
	_button(button),
	_state(false),
	_postState(false),
	_msg(SafetySamVoice::ERROR)
{
}

void PlayMessages::begin() {
	_button->begin();
}

boolean PlayMessages::update() {
	uint8_t buttonState;
	boolean rv;
	
	// Current Button State
	buttonState = _button->update();
	
	if( buttonState == Button2::PRESS || buttonState == Button2::RAPID_PRESS ) {
		_state = true;
		rv = true;
	}
	else {
		rv = false;
	}
	
	if(_state && (rv || buttonState == Button2::PRESSING) )
		_postState = true;
	else
		_postState = false;
	
	return rv;
}

boolean PlayMessages::postUpdate() {
	return _postState;
}

boolean PlayMessages::isProcessing() {
	return _button->isProcessing() || (_postState && hasMessage());
}

void PlayMessages::setMessage(uint16_t msg) {
	_msg = msg;
}

boolean PlayMessages::hasMessage() {
	if(_msg == SafetySamVoice::ERROR)
		return false;
	return true;
}

uint16_t PlayMessages::getMessage() {
	return _msg;
}

void PlayMessages::clearMessage() {
	_state = _postState = false;
	_msg = SafetySamVoice::ERROR;
}

uint8_t PlayMessages::getState() {
	return _state ? 1 : 0;
}