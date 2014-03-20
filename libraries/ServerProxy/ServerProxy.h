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
		boolean isWaitingToFlush();
		boolean willOverflowOutgoing(uint32_t numBits);
		boolean setOutgoing(uint32_t data, uint8_t numBits);
		void endOutgoing();
		void resetOutgoing();
		unsigned long getLastOutgoingSentTime();
		const char* getIncoming();
		void resetIncoming();
		
		//WIFI STATES
		static const uint8_t CLOSED = 0;
		static const uint8_t BOOTING = 1;
		static const uint8_t READY = 2;
		static const uint8_t OPENED = 3;
		
	//------------------------------------------------------------------------------
	private:
		void bootup();
		boolean setIncoming( char in );
		
		//wifi state
		uint8_t _state;
	
		//both outgoing & incoming
		static const uint8_t LAST_INDEX = 254; //need one for \0
	
		//outgoing
		static const uint16_t LAST_BIT_INDEX = 2032; //254*8
		uint32_t _outLengthInBits;
		uint8_t _out[256];
		boolean _needToFlush;
		
		//Serial interface to send/receive data to server
		Stream *_serial;
		
		//Serial interface to use for debugging
		Stream *_debug;
		
		//incoming
		uint8_t _inLen;
		uint8_t _in[256];
		uint8_t _inState;
		uint16_t _commandParsePosition;
		
		//incoming states
		static const uint8_t WIFI_STATE_FOUND = 0;
		static const uint8_t COMMAND_FOUND = 1;
		static const uint8_t COMMAND_NOT_FOUND = 2;
		
		//wifi constants
		static char WIFI_BOOTUP;
		static const char INCOMING_WIFI_START = '*';
		static const char INCOMING_READY = 'R';
		static const char INCOMING_OPENED = 'O';
		static const char INCOMING_CLOSED = 'C';
		
		//data constants
		static const char INCOMING_DATA_START[];
		static const unsigned int INCOMING_DATA_START_LENGTH;
		static const char DATA_END[];
		static const unsigned int DATA_END_LENGTH;
		
		//timestamps
		unsigned long _last_outgoing_sent_time;
};

#endif // SERVER_PROXY_H