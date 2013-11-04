#include "Wtv020sd16p.h"

Wtv020sd16p::Wtv020sd16p(uint8_t resetPin, uint8_t clockPin, uint8_t dataPin, uint8_t busyPin) :
	Audio(resetPin, clockPin, dataPin, busyPin)
{
}

void Wtv020sd16p::begin(){
  digitalWrite(_clockPin, LOW);
  digitalWrite(_resetPin, HIGH);
  //Reset pulse.
  digitalWrite(_resetPin, LOW);
  delay(5);
  digitalWrite(_resetPin, HIGH);
  //Reset idle to start bit. 
  digitalWrite(_clockPin, HIGH);
  delay(750);
  
  Audio::begin();
}

void Wtv020sd16p::sendCommand(unsigned int command) {
  //Start bit Low level pulse.
  digitalWrite(_clockPin, LOW);
  delay(2);
  for (unsigned int mask = 0x8000; mask > 0; mask >>= 1) {
    //Clock low level pulse.
    digitalWrite(_clockPin, LOW);
    delayMicroseconds(50);
    //Write data setup.
    if (command & mask) {
      digitalWrite(_dataPin, HIGH);
    }
    else {
      digitalWrite(_dataPin, LOW);
    }
    //Write data hold.
    delayMicroseconds(50);
    //Clock high level pulse.
    digitalWrite(_clockPin, HIGH);
    delayMicroseconds(100);
    if (mask>0x0001){
      //Stop bit high level pulse.
      delay(2);      
    }
  }
  //Busy active high from last data bit latch.
  delay(50);
}