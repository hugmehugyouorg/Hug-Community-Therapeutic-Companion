#include <QuietTime.h>

QuietTime::QuietTime(Button2 *quiteTimeButton) :
	_quiteTimeButton(quiteTimeButton),
	_state(false)
{
}

void QuietTime::begin() {
	_quiteTimeButton->begin();
}

boolean QuietTime::update() {
	uint8_t quietTimeButtonState;
	boolean rv;
	
	// Current Button State
	quietTimeButtonState = _quiteTimeButton->update();
	
	if( quietTimeButtonState == Button2::PRESS || quietTimeButtonState == Button2::RAPID_PRESS ) {
		_state = true;
		rv = true;
	}
	else {
		rv = false;
	}
	
	return rv;
}

boolean QuietTime::isProcessing() {
	return _quiteTimeButton->isProcessing();
}

boolean QuietTime::isEnabled() {
	return _state;
}

void QuietTime::end() {
	_state = false;
}

uint8_t QuietTime::getState() {
	return _state ? 1 : 0;
}