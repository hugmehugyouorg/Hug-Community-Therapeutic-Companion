#ifndef SERVER_PROXY_H
#define SERVER_PROXY_H

#include <Arduino.h>
#include <Stream.h>

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
		
	//------------------------------------------------------------------------------
	private:
		void bootup();
		boolean setIncoming( char in );

		//wifi states we are in
		uint8_t _state;
		static const uint8_t CLOSED_STATE_FOUND = 0;
		static const uint8_t BOOTING_STATE_FOUND = 1;
		static const uint8_t READY_STATE_FOUND = 2;
		static const uint8_t OPENED_STATE_FOUND = 3;

		//both outgoing & incoming
		static const uint8_t LAST_INDEX = 254; //need one for \0
	
		//outgoing
		static const uint16_t LAST_BIT_INDEX = 2032; //254*8
		uint32_t _outLengthInBits;
		uint8_t _out[256];

		//outgoing states we are in
		boolean _needToFlush;

		//timestamps
		unsigned long _last_outgoing_sent_time;
		unsigned long _bootup_timeout;
		unsigned long _heartbeat_timeout;

		//WATCHDAWGS
		static const unsigned long BOOTUP_WATCHDAWG_SHOULD_BITE = 30000; //1 = 1ms, 1000 = 1 second, 30000 = 30 seconds, 300000 = 5 minutes
		static const unsigned long HEARTBEAT_WATCHDAWG_SHOULD_BITE = 6000;

		//Serial interface to send/receive data to server
		Stream *_serial;
		
		//Serial interface to use for debugging
		Stream *_debug;
		
		//incoming
		uint8_t _inLen;
		uint8_t _in[256];
		uint16_t _commandParsePosition;
		
		//incoming states (either wifi telling us something or it is a server command)
		uint8_t _inState;
		static const uint8_t WIFI_STATE_FOUND = 0;
		static const uint8_t COMMAND_FOUND = 1;
		static const uint8_t COMMAND_NOT_FOUND = 2;

		//wifi constants
		static char WIFI_BOOTUP;
		static const char WIFI_START = '*';
		static const char OPENED = 'O';
		static const char CLOSED = 'C';
		
		//data constants
		static const char DATA_START[];
		static const uint16_t DATA_START_LENGTH;
		static const char DATA_END[];
		static const uint16_t DATA_END_LENGTH;
};

#endif // SERVER_PROXY_H