#include <BatteryFioVThree.h>
#include <Button2.h>
#include <QuietTime.h>
#include <Emotion.h>
#include <Audio.h>
#include <Wtv020sd16p.h>
#include <SafetySamVoice.h>
#include <ServerProxy.h>
#include <SafetySam.h>

//only in debugging
//#include <MemoryFree.h>

//button pins used
#define HAPPY_PIN 9
#define UNHAPPY_PIN 10
#define QUIET_TIME_PIN 8

//audio pins used
#define AUDIO_RESET_PIN 4
#define AUDIO_CLOCK_PIN 5
#define AUDIO_DATA_PIN 6
#define AUDIO_BUSY_PIN 7

//buttons
Button2 btnHappy(HAPPY_PIN);
Button2 btnUnhappy(UNHAPPY_PIN);
Button2 btnQuietTime(QUIET_TIME_PIN);

//quiet time
QuietTime quietTime(&btnQuietTime);

//emotions
Emotion emotion(&btnHappy, &btnUnhappy);

//audio player
Wtv020sd16p player(AUDIO_RESET_PIN, AUDIO_CLOCK_PIN, AUDIO_DATA_PIN, AUDIO_BUSY_PIN);

//safety sam voice
SafetySamVoice voice(&player);

//battery monitor
BatteryFioVThree battery;

//Server proxy
//ServerProxy proxy(&Serial1); //Fio V3 w/o debugging
ServerProxy proxy(&Serial1, &Serial); //Fio V3 w/ debugging
//ServerProxy proxy(&Serial); //No wifi, just Computer

//safety sam
//SafetySam safetySam(&voice, &emotion, &quietTime, &proxy, (Battery*) &battery); // w/o debugging
SafetySam safetySam(&voice, &emotion, &quietTime, &proxy, (Battery*) &battery, &Serial); // w/ debugging

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);
  //Serial.println(freeMemory());
  safetySam.begin();
}

void loop(){
  safetySam.update();
  //Serial.println(freeMemory());
}



