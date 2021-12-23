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
    p_ringBands,
    p_ringBands_inverted,
    p_hexBands,
    // patternFive,
    p_spiralIn,
    p_vuMeter,
    p_vuMeter_inverted,
    p_spinTheWheel,
    p_fadeWheel};
int NUMPATTERNS = sizeof(allPatterns) / sizeof(PatternFunction);

typedef void (*IdleFunction)();
IdleFunction idlePatterns[] = {
    // ip_spiralout_spiralin,  // started crashing and I don't like it enough to care
    ip_spiralout,
    ip_rainbowrings,
    ip_rainbowhexes,
    ip_outline,
    ip_spinner_rainbow,
    // ip_spiralout_rainbow,
    ip_spiral_rgb,
    ip_spiral_matrix,
    ip_white_spinner,
    ip_chase_rgb,

    // below were cribbed from FastLED examples
    FireNormal,
    FireCool,
    pacifica_loop
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
      if (idleTimer / 1000 > IDLE_TIMEOUT)
      {
        FastLED.setBrightness(IDLEBRIGHTNESS);
        patternSwitch = patternSwitch - 10; // slow down a bit if we're in idle mode
        idlePatterns[idleIndex]();
      }
      else
      {
        FastLED.setBrightness(BRIGHTNESS );
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
