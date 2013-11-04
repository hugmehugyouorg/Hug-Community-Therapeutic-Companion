#include <Button2.h>

Button2::Button2(uint8_t buttonPin, uint8_t groundPin) :
    _buttonPin(buttonPin),
    _groundPin(groundPin)
{
	_state = NO_CONTACT_DETECTED;
	_firstPress = 0;
	_lastPress = 0;
	_presses = 0;
	_trackPresses = true;
}

void Button2::begin() {
    pinMode(_buttonPin, INPUT_PULLUP);
    //make pin ground
    if(_groundPin != 255) {
  		pinMode(_groundPin, OUTPUT);
  		digitalWrite(_groundPin, LOW);
  	}
}

boolean Button2::isProcessing() {
	//if not tracking presses anymore than we are not processing
	if(!_trackPresses)
		return false;
	
	//if no contact and no presses detected so far then not processing
	if(_state == NO_CONTACT_DETECTED && _presses == 0)
		return false;
		
	//all other states while tracking presses means we are processing
	return true;
}

unsigned long Button2::timeElapsedFirstPress() {
	if(_firstPress == 0)
		return 0;
	return millis() - _firstPress;
}

unsigned long Button2::timeElapsedLastPress() {
	if(_lastPress == 0)
		return timeElapsedFirstPress();
	return millis() - _lastPress;
}

uint8_t Button2::update() {
	uint8_t rv;
	
	//determine the current state based on the previous state and the current button
	switch(digitalRead(_buttonPin)) {
	
	 // Keep in mind the pullup means the pushbutton's
  	 // logic is inverted. It goes HIGH when it's open,
     // and LOW when it's pressed:

		case HIGH:
			switch(_state) {
				case NO_CONTACT_DETECTED:
				case CONTACT_DETECTED:
					_state = NO_CONTACT_DETECTED;
					rv = _state;
					break;
				case PRESSING:
					_state = NO_CONTACT_DETECTED;
					rv = _state;
					break;
			}
			
			//if we were tracking presses and time has elapsed from either last press or time has elapsed from the first press
			//then it was just a regular press we detected
			if(_trackPresses && timeElapsedLastPress() > RAPID_PRESSES_DELAY || timeElapsedFirstPress() > RAPID_PRESSES_WINDOW) {
				rv = PRESS;
				_firstPress = 0;
				_lastPress = 0;
				_presses = 0;
			}
			_trackPresses = true;
			break;
		case LOW:
			switch(_state) {
				case NO_CONTACT_DETECTED:
					_state = CONTACT_DETECTED;
					_pressDeadline = millis() + PRESS_DEBOUNCE;
					rv = _state;
					break;
				case CONTACT_DETECTED:
					if (millis() >= _pressDeadline) {
                		if(_presses++ == 0)
							_firstPress = millis();
						else
							_lastPress = millis();
							
						_state = PRESSING;
                	}
                	rv = _state;
					break;
			}
			
			if(_trackPresses) {
				//if correct number of presses for rapid pressing and the time elapsed from the first press is within the 
				//correct window of time to be considered rapid then it is a rapid press so stop tracking presses while
				//pressing
				if(_presses == RAPID_PRESSES && timeElapsedFirstPress() <= RAPID_PRESSES_WINDOW) {
					rv = RAPID_PRESS;
					_firstPress = 0;
					_lastPress = 0;
					_presses = 0;
					_trackPresses = false;
				} //else if time has elapsed between the last press or time elapsed from the first press is outside the
				//correct window of time to be considered rapid then it was just a regular press so stop tracking
				//presses while pressing
				else if(timeElapsedLastPress() > RAPID_PRESSES_DELAY || timeElapsedFirstPress() > RAPID_PRESSES_WINDOW) {
					rv = PRESS;
					_firstPress = 0;
					_lastPress = 0;
					_presses = 0;
					_trackPresses = false;
				}
			}
			
			break;
	}
	
	return rv;
}