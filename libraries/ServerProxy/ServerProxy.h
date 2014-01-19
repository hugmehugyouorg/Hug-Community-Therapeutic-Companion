#ifndef SERVER_PROXY_H
#define SERVER_PROXY_H

#include <Arduino.h>
#include <Stream.h>
#include "../Command/Command.h"

/*
OUTGOING (wifi device auto-adding: GET$/?i=DEVICE_ID&d=):

THIS_IS_THE_ACTUAL_DATA

*/

/*
INCOMING:

HTTP/1.1 207 THIS_IS_THE_ACTUAL_DATA\n...Connection: close\n\n...

*/

class ServerProxy {
	public:
	
		ServerProxy(Stream *serial, Stream *debugPrint = NULL);
		void begin();
		boolean update();
		boolean isProcessing();
		void startOutgoing();
		boolean willOverflowOutgoing(uint32_t numBits);
		boolean setOutgoing(uint32_t data, uint8_t numBits);
		void endOutgoing();
		void resetOutgoing();
		const char* getIncoming();
		void resetIncoming();
		
	//------------------------------------------------------------------------------
	private:
		boolean setIncoming( char in );
	
		//both outgoing & incoming
		const static uint8_t LAST_INDEX = 254; //need one for \0
	
		//outgoing
		const static uint16_t LAST_BIT_INDEX = 2032; //254*8
		uint32_t _outLengthInBits;
		uint8_t _out[256];
		boolean _outgoing;
		
		//Serial interface to send/receive data to server
		Stream *_serial;
		
		//Serial interface to use for debugging
		Stream *_debug;
		
		//incoming
		uint8_t _inLen;
		uint8_t _in[256];
		uint8_t _inState;
		uint16_t _commandParsePosition;
		
		const static uint8_t COMMAND_FOUND = 0;
		const static uint8_t COMMAND_NOT_FOUND = 1;
		
		//parser constants
		const static char INCOMING_DATA_START[];
		const static unsigned int INCOMING_DATA_START_LENGTH;
		const static char DATA_END[];
		const static unsigned int DATA_END_LENGTH;
};

#endif // SERVER_PROXY_H