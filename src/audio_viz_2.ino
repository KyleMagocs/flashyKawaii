// MY
// FUCKIN
// AUDIO
// VIZ

#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <WS2812Serial.h>
#define USE_WS2812SERIAL // the audio shield haaaaates you if you don't do this
#define FASTLED_INTERNAL
#include <FastLED.h>

#include "Hex.h"
#include "Patterns.h"
#include "IdlePatterns.h"
#include "AudioControl.h"
#include "constants.h"

typedef void (*PatternFunction)(float levels[]);
PatternFunction allPatterns[] = {
    // patternOne,
    // patternTwo,
    patternThree,
    patternFour,
    patternFive,
    patternSix,
    patternSeven,
    patternEight,
    patternNine,
    patternTen};
int NUMPATTERNS = sizeof(allPatterns) / sizeof(PatternFunction);

typedef void (*IdleFunction)();
IdleFunction idlePatterns[] = {
    // IdlePatternOne_better,  // started crashing and I don't like it enough to care
    IdlePatternTwo,
    IdlePatternThree,
    IdlePatternFour_fullsend,
    // IdlePatternFive,
    pacifica_loop,
    FireNormal,
    FireCool,
    TestPatternFour,
    TestPatternThree,
    TestPatternThree_Matrix,
    TestPatternFive
    };
int NUMIDLEPATTERNS = sizeof(idlePatterns) / sizeof(IdleFunction);

int patternIndex;
int idleIndex;

void printBands()
{
  int bands = 8;
  int bins = 256;
  for (int b = 0; b < bands; b++)
  {
    int from = int(exp(log(bins) * b / bands));
    int to = int(exp(log(bins) * (b + 1) / bands));
    if (from == 0)
      Serial.printf("%1d %1d\n", from, to - 2); // maual tweaking off the beaten path woo
    else
      Serial.printf("%1d %1d\n", from - 1, to - 2);
    // Serial.printf("To   = %1d\n", to-1);
  }
  Serial.println("OR!!!");
  bins = 1024;
  for (int b = 0; b < bands; b++)
  {
    int from = int(exp(log(bins) * b / bands));
    int to = int(exp(log(bins) * (b + 1) / bands));
    Serial.printf("%1d, %1d\n", from-1, to); // maual tweaking off the beaten path woo
    
    // Serial.printf("To   = %1d\n", to-1);
  }
}

void setup()
{
  delay(1000);
  Serial.begin(9600);
  // printBands();
  randomSeed(analogRead(0));
  patternIndex = random(NUMPATTERNS);
  idleIndex = random(NUMIDLEPATTERNS);
  Serial.printf("pattern:%d ", patternIndex);
  Serial.printf("idle:%d ", idleIndex);

  init_leds();
  startupTest();

  init_audio();
  init_hexes();
}

elapsedMillis drawTimer;
elapsedMillis idleTimer = 30000; // just so it starts up in idle mode
elapsedMillis patternSwitch;

// todo: move to patterns.h

PatternFunction curPattern;

void loop()
{
  if (drawTimer > 1000 / FPS)
  {
    // beatDetectionLoop();  // I'm pretty sure all this really does is slow everything down, so let's not.
    if (fft256_1.available())
    {
      drawTimer = 0;
      fadeAllLeds();
      killTheExtras(); // have yet to figure out why I gotta do this.
      float levels[8];
      LoadLevels1024(levels);
      bassHit(levels);
      if (patternSwitch / 1000 > PATTERN_TIMER)
      {
        idleIndex = random(NUMIDLEPATTERNS);
        patternIndex = random(NUMPATTERNS);
        Serial.printf("pattern:%d ", patternIndex);
        Serial.printf("idle:%d ", idleIndex);
        patternSwitch = 0;
        increaseHue(random(128)); // randomize our manual palette
        randomizePalette();  // randomize the index of our pre-def palettes
        decayFactor = DECAY; // reset it, let patterns choose to change it.
        Serial.println();
      }

      if (levels[0] > 1)
      {
        idleTimer = 0;
      }
      if (idleTimer / 1000 > 30)
      {
        FastLED.setBrightness(IDLEBRIGHTNESS);
        patternSwitch = patternSwitch - 10; // slow down a bit if we're in idle mode
        idlePatterns[idleIndex]();
      }
      else
      {
        FastLED.setBrightness(BRIGHTNESS );
        // patternTen(levels);
        allPatterns[patternIndex](levels);
      }
      FastLED.show();
    }
    else
    {
      //    Serial.println("fft not available???");
    }
  }
}
