#include <ServerProxy.h>

const char ServerProxy::OUTGOING_DATA_START[] = {'D', ':'};
const unsigned int ServerProxy::OUTGOING_DATA_START_LENGTH = sizeof(OUTGOING_DATA_START) / sizeof(OUTGOING_DATA_START[0]);
const char ServerProxy::INCOMING_DATA_START[] = {'H', 'T', 'T', 'P', '/', '1', '.', '1', ' ', '2', '0', '7', ' '};
const unsigned int ServerProxy::INCOMING_DATA_START_LENGTH = sizeof(INCOMING_DATA_START) / sizeof(INCOMING_DATA_START[0]);
const char ServerProxy::DATA_END[] = {'\n'};
const unsigned int ServerProxy::DATA_END_LENGTH = sizeof(DATA_END) / sizeof(DATA_END[0]);

ServerProxy::ServerProxy(Stream *serial, Stream *debug) :
	_serial(serial),
	_debug(debug)
{
	resetOutgoing();
	resetIncoming();
}

void ServerProxy::begin() {
}

boolean ServerProxy::update() {
	char in;
	while(_serial->available() > 0) {
		//if there is data to be processed then process it
		in = (char)_serial->read();
		if( setIncoming( in ) ) {
			return true;
		}
	}
	return false;
}

boolean ServerProxy::isProcessing() {
	return (_serial->available() > 0);
}

void ServerProxy::startOutgoing() {
	_outgoing = true;
}

boolean ServerProxy::willOverflowOutgoing(uint32_t numBits) {
	//numBits = ceil(numBits / 8.0) * 8;
	return (_outLengthInBits + numBits) > LAST_BIT_INDEX;
}

boolean ServerProxy::setOutgoing(uint32_t out, uint8_t numBits) {
	//numBits = ceil(numBits / 8.0) * 8;
	
	if(willOverflowOutgoing(numBits))
		return false;
		
	if(!_outgoing)
		startOutgoing();
	
	uint8_t byteIndex;
	uint8_t bitIndex;
	uint8_t bitsLeft = numBits;
	
	if(_debug != NULL) {
		_debug->print("outgoing data: ");
		_debug->print(out);
		_debug->println("");
	}
	
	//while the output buffer is not full AND
	//there is still more bits to add
	//NOTE: MSB FIRST, 2 = 01, 4 = 001, etc.
	while(_outLengthInBits < LAST_BIT_INDEX && bitsLeft != 0) {
		byteIndex = floor(_outLengthInBits / 8.0);
		bitIndex = _outLengthInBits % 8;
		
		if(_debug != NULL) {
			//_debug->print("before: ");
			//_debug->println((out & 1) == 0 ? "0" : "1");
			_debug->print((out & 1) == 0 ? "0" : "1");
		}
		_out[byteIndex] |= (out & 1) << bitIndex;
		
		/*if(_debug != NULL) {
			_debug->print("after: ");
			_debug->println((((_out[byteIndex]) >> bitIndex) & 1));
		}*/
		
		//update out by readying the next bit
		out >>= 1;
		
		//update the output buffer length in bits
		_outLengthInBits++;
		
		//update the bits left
		bitsLeft--;
	}
	
	if(_debug != NULL)
		_debug->println("");
	
	if(bitsLeft == 0)
		return true;
		
	//this should never happen
	return false;
}

void ServerProxy::endOutgoing() {
	uint8_t bits = 0;
	uint8_t bitIndex = 0;
	uint8_t byteIndex = 0;
	uint8_t out = 0;
	uint8_t bit = 0;
	uint8_t bytesWritten = 0;
	
	while(bits < _outLengthInBits) {
		byteIndex = floor(bits / 8.0);
		
		out = _out[byteIndex];
		bitIndex = 0;
		
		if(_debug != NULL)
			_debug->print("bits: ");
			
		while(bitIndex < 8) {
			bit = (out & 1);
			
			if(_debug != NULL)
				_debug->print(bit == 0 ? "0" : "1");
			out >>= 1;
			bitIndex++;
		}
		if(_debug != NULL)
			_debug->println("");
		
		bits+=8;
	}
	
	if(_debug != NULL)
		_debug->println("");
	
	if(_debug != NULL) {
		_debug->print("bits length: ");
		_debug->println(_outLengthInBits);
	}
	
	bytesWritten = ceil(_outLengthInBits/8.0);
	
	if(_debug != NULL) {
		_debug->print("bytes to write: ");
		_debug->println(bytesWritten);
	}
	
	bytesWritten = _serial->write(_out, bytesWritten);
	
	if(_debug != NULL) {
		_debug->print("bytes written: ");
		_debug->println(bytesWritten);
	}
		
	resetOutgoing();
}

const char* ServerProxy::getIncoming() {
	return (char *)_in;
}

void ServerProxy::resetOutgoing() {
	memset(_out,0,sizeof(_out));
	_outLengthInBits = 0;
	_outgoing = false;
}

void ServerProxy::resetIncoming() {
	memset(_in,0,sizeof(_in));
	_inLen = 0;
	_inState = COMMAND_NOT_FOUND;
	_commandParsePosition = 0;
}

boolean ServerProxy::setIncoming( char in ) {
	if(_debug != NULL)
		_debug->print(in);
		
	if(_inState == COMMAND_NOT_FOUND) {
		//see if incoming matches command syntax
		if(in == INCOMING_DATA_START[_commandParsePosition++]) {
			//if command found then switch incoming state and reset command parse position for the next time
			if(_commandParsePosition == INCOMING_DATA_START_LENGTH) {
				_inState = COMMAND_FOUND;
			}
		} //the data does not match the protocol, so start looking for the command again
		else {
			_commandParsePosition = 0;
		}
	} //we can't handle anymore, doesn't follow protocol, so start looking for the command again
	else if(_inLen > LAST_INDEX) {
		resetIncoming();
	} //else collect the data
	else if(in != DATA_END[0]) {
		_in[_inLen++] = in;
	}
	else {
		//data found and at the end of the data
		_inState = COMMAND_NOT_FOUND;
		_commandParsePosition = 0;
		if(_debug != NULL) {
			_debug->println("");
			_debug->print("COMMAND FOUND: ");
			_debug->write(_in, _inLen);
			_debug->println("");
		}
		return true;
	}
	return false;
}