#include <SafetySamVoice.h>

SafetySamVoice::SafetySamVoice(Audio *player) :
	_player(player)
{
	_lastSaid = NONE;
	_lastMessageSaid = NONE;
}

void SafetySamVoice::begin() {
	_player->begin();
}

void SafetySamVoice::readyToPlay() {
	randomPlay(READY_TO_PLAY_OFFSET, READY_TO_PLAY_LENGTH);
}

void SafetySamVoice::batteryLow() {
	randomPlay(BATTERY_LOW_OFFSET, BATTERY_LOW_LENGTH);
}

void SafetySamVoice::batteryCharged() {
	randomPlay(BATTERY_CHARGED_OFFSET, BATTERY_CHARGED_LENGTH);
}

void SafetySamVoice::quietTime() {
	randomPlay(QUITE_TIME_BUTTON_CLICKED_OFFSET, QUITE_TIME_BUTTON_CLICKED_LENGTH);
}

void SafetySamVoice::happy() {
	randomPlay(HAPPY_BUTTON_CLICKED_OFFSET, HAPPY_BUTTON_CLICKED_LENGTH);
}

void SafetySamVoice::unhappy() {
	randomPlay(UNHAPPY_BUTTON_CLICKED_OFFSET, UNHAPPY_BUTTON_CLICKED_LENGTH);
}

void SafetySamVoice::emergency() {
	randomPlay(UNHAPPY_BUTTON_RAPIDLY_CLICKED_OFFSET, UNHAPPY_BUTTON_RAPIDLY_CLICKED_LENGTH);
}

boolean SafetySamVoice::canSay(uint16_t message) {
	return message >= STANDARD_MESSAGE_OFFSET && message < STANDARD_MESSAGE_OFFSET + STANDARD_MESSAGE_LENGTH || message >= SAFETY_TIME_MESSAGE_EVENTS_OFFSET && message < SAFETY_TIME_MESSAGE_EVENTS_OFFSET + SAFETY_TIME_MESSAGE_EVENTS_LENGTH;
}

boolean SafetySamVoice::canSay(const char* message) {
	return _player->canPlay(message) && canSay( _player->toId(message) );
}

uint16_t SafetySamVoice::strMsgToUintMsg(const char* message) {
	if(canSay(message))
		return _player->toId(message);
	return ERROR;
}

boolean SafetySamVoice::say(uint16_t message) {
	if( canSay(message) ) {
		_player->play(message);
		_lastMessageSaid = message;
		return true;
	}
	return false;
}

boolean SafetySamVoice::say(const char* message) {
	if( canSay(message) ) {
		return say( _player->toId(message) );
	}
	return false;
}

uint16_t SafetySamVoice::randomPlay(uint16_t offset, uint16_t length) {
	randomSeed( millis() );
	uint16_t message = (uint16_t) random(offset, offset + length);
	_player->play(message);
	_lastSaid = message;
	return message;
}

uint16_t SafetySamVoice::getLastSaid() {
	return _lastSaid;
}

uint16_t SafetySamVoice::getLastMessageSaid() {
	return _lastMessageSaid;
}

boolean SafetySamVoice::isProcessing() {
	return _player->isPlaying();
}