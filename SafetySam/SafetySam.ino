/**
*
*  Since we are currently using FioV3 which defines __AVR_ATmega32U4__
*  then the following configuration is required for PinChangeInt to work correctly.
*
**/

// You can reduce the memory footprint of this handler by declaring that there will be no pin change interrupts
// on any one or two of the three ports.  If only a single port remains, the handler will be declared inline
// reducing the size and latency of the handler.
//#define NO_PORTB_PINCHANGES // to indicate that port b will not be used for pin change interrupts
#define NO_PORTC_PINCHANGES // to indicate that port c will not be used for pin change interrupts
//#define NO_PORTD_PINCHANGES // to indicate that port d will not be used for pin change interrupts
// *** New in version 1.5: ********************************
// You can reduce the code size by 20-50 bytes, and you can speed up the interrupt routine
// slightly by declaring that you don't care if the static variables PCintPort::pinState and/or
// PCintPort::arduinoPin are set and made available to your interrupt routine.
#define NO_PIN_STATE        // to indicate that you don't need the pinState
#define NO_PIN_NUMBER       // to indicate that you don't need the arduinoPin
// *********************************************************
// if there is only one PCInt vector in use the code can be inlined
// reducing latency and code size
// define DISABLE_PCINT_MULTI_SERVICE below to limit the handler to servicing a single interrupt per invocation.
#define DISABLE_PCINT_MULTI_SERVICE
#include <PinChangeInt.h>

#include <BatteryFioVThree.h>
#include <Button2.h>
#include <PlayMessages.h>
#include <Emotion.h>
#include <Audio.h>
#include <Wtv020sd16p.h>
#include <SafetySamVoice.h>
#include <ServerProxy.h>
#include <SafetySam.h>
#include <LowPower.h>

//only if debugging memory
//#include <MemoryFree.h>

//whether to be in debug mode or not
//#define SAFETY_SAM_DEBUG 1

//wifi pin (RX/UART Receive)
#define WIFI_PIN 0

//button pins used
#define HAPPY_PIN 9
#define UNHAPPY_PIN 10
#define PLAY_MESSAGES_PIN 8

//audio pins used
#define AUDIO_RESET_PIN 4
#define AUDIO_CLOCK_PIN 5
#define AUDIO_DATA_PIN 6
#define AUDIO_BUSY_PIN 7

//buttons
Button2 btnHappy(HAPPY_PIN);
Button2 btnUnhappy(UNHAPPY_PIN);
Button2 btnPlayMessages(PLAY_MESSAGES_PIN);

//play messages
PlayMessages playMessages(&btnPlayMessages);

//emotions
Emotion emotion(&btnHappy, &btnUnhappy);

//audio player
Wtv020sd16p player(AUDIO_RESET_PIN, AUDIO_CLOCK_PIN, AUDIO_DATA_PIN, AUDIO_BUSY_PIN);

//safety sam voice
SafetySamVoice voice(&player);

//battery monitor
BatteryFioVThree battery;

#ifdef SAFETY_SAM_DEBUG

  //Server proxy
  ServerProxy proxy(&Serial1, &Serial);
  
  //safety sam
  SafetySam safetySam(&voice, &emotion, &playMessages, &proxy, (Battery*) &battery, &Serial);
  
  boolean processing = false;
  
#else

  //Server proxy
  ServerProxy proxy(&Serial1);
  
  //safety sam
  SafetySam safetySam(&voice, &emotion, &playMessages, &proxy, (Battery*) &battery);
  
#endif

boolean interruptsAttached = false;

void wifiInterrupt() {
 
}
void happyInterrupt() {
 
}
void unHappyInterrupt() {
 
}
void playInterrupt() {
 
}

boolean pcAttachInterrupt() {
  if(!interruptsAttached)
  {
      int rr = PCintPort::attachInterrupt(WIFI_PIN, &wifiInterrupt, CHANGE);
      int pr = PCintPort::attachInterrupt(PLAY_MESSAGES_PIN, &playInterrupt, CHANGE);
      int hr = PCintPort::attachInterrupt(HAPPY_PIN, &happyInterrupt, CHANGE);
      int ur = PCintPort::attachInterrupt(UNHAPPY_PIN, &unHappyInterrupt, CHANGE);
      
      if(rr == 1 && pr == 1 && hr == 1 && ur == 1)
      {
          interruptsAttached = true;
          
          #ifdef SAFETY_SAM_DEBUG
              Serial.println("Attached interrupts.");
          #endif
      }
      else {
          #ifdef SAFETY_SAM_DEBUG
              Serial.println("Couldn't attach interrupts.");
              Serial.print("wifi Pin attach result: ");
              Serial.println(rr);
              Serial.print("Play Messages Pin attach result: ");
              Serial.println(pr);
              Serial.print("Happy Pin attach result: ");
              Serial.println(hr);
              Serial.print("Unhappy Pin attach result: ");
              Serial.println(ur);
          #endif 
      }
  }
  
  return interruptsAttached;
}

void pcDetachInterrupt() {
  PCintPort::detachInterrupt(WIFI_PIN);
  PCintPort::detachInterrupt(PLAY_MESSAGES_PIN);
  PCintPort::detachInterrupt(HAPPY_PIN);
  PCintPort::detachInterrupt(UNHAPPY_PIN);
  #ifdef SAFETY_SAM_DEBUG
    Serial.println("Detached interrupts.");
  #endif
}

void goToSleep()
{
    if(!pcAttachInterrupt())
    {
        //pcDetachInterrupt();
    }
    else
    {
        #ifdef SAFETY_SAM_DEBUG
          //Serial.println("Going to sleep.");
        #endif
        
        /**
        * IMPORTANT!!!  ALERT!!!  WARNING!!!  
        *
        *    Need to correctly add interrupts or this will brick the fio v3 currently and requires the double reset / 8 second upload sketch fix.
        *    Be careful turning off the USB port as that is how we program a fio v3.  Using idle with it on is about as good as it gets with fio v3,
        *    if you still want to program things.  So while I've been able to get Safety Sam to work with below, to repogram requires 2x reset fix.
        *   
        *    // Enter power down state with ADC and BOD modules disabled.
        *    // Mini Ultra 8 MHz uses 1.7 µA if BOD_OFF & 18.6 µA if on
        *    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);  
        *
        * DESCRIPTION:
        *
        *     See http://www.rocketscream.com/blog/2011/07/04/lightweight-low-power-arduino-library/
        *     Mini Ultra 8 MHz uses probably between 687.0 µA - 832.0 µA with this sort of config, not sure about fio v3 though
        *     See http://harizanov.com/2013/02/power-saving-techniques-on-the-atmega32u4/ for a start though since that's the basis
        *     9.97mA - 12.3mA
        *     TIMER0_ON for millis() function to work properly when determining safety sam watchdawg
        *     USB_ON for debugging, but most especially cause otherwise can't set new firmware without a hard reset (dbl tap reset button)
        **/
        LowPower.idle(SLEEP_FOREVER, ADC_OFF, TIMER4_OFF, TIMER3_OFF, TIMER1_OFF, TIMER0_ON, SPI_OFF, USART1_ON, TWI_OFF, USB_ON);
        //pcDetachInterrupt();
        
        #ifdef SAFETY_SAM_DEBUG
          //Serial.println("Woke from sleep");
        #endif
    }
}

void setup(){
  Serial1.begin(9600);
  
  #ifdef SAFETY_SAM_DEBUG
    Serial.begin(9600);
  #endif
  
  waitForSerialMonitor();
  
  //Serial.println(freeMemory());
  safetySam.begin();
}

void loop(){
  safetySam.update();
  
  if(safetySam.isProcessing())
  {
      #ifdef SAFETY_SAM_DEBUG
        if(!processing)
          Serial.println("processing");
        processing = true;
      #endif
  }
  else 
  {
      #ifdef SAFETY_SAM_DEBUG
          if(processing)
          {
            Serial.println("not processing");
          }
          processing = false;
      #endif
      
      goToSleep();
  }
      
  //Serial.println(freeMemory());
}

void waitForSerialMonitor() {
 // allow a little time to connect the serialMonitor before running the rest of the setup.
  for (int i = 10; i>0; i--) {
    delay(1000);
    #ifdef SAFETY_SAM_DEBUG
      Serial.print(F(" "));
      Serial.print(i);
    #endif
  } 
  #ifdef SAFETY_SAM_DEBUG
    Serial.println(F(" "));
    Serial.println(F("------- SERIAL MONITOR READY -------"));
  #endif
}

