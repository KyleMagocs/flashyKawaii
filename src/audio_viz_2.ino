// MY
// FUCKIN
// AUDIO
// VIZ


#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <WS2812Serial.h>
#define USE_WS2812SERIAL  // the audio shield haaaaates you if you don't do this
#define FASTLED_INTERNAL
#include <FastLED.h>

#include "Hex.h"
#include "Patterns.h"
#include "AudioControl.h"

#define PATTERN_TIMER 30

typedef void (*PatternFunction) (float levels[]);
PatternFunction allPatterns[] = {
  patternOne,
  patternTwo,
  patternThree,
  patternFour,
  patternFive,
  patternSix,
  patternSeven,
  patternEight
};

typedef void (*IdleFunction) ();
IdleFunction idlePatterns[] = {
  IdlePatternOne,
  IdlePatternTwo,
  IdlePatternThree,
  IdlePatternFour,
  IdlePatternFive
};

int patternIndex;
int idleIndex;

void setup() {
  randomSeed(analogRead(0));
  patternIndex = random(8);
  idleIndex = random(5);
  init_leds();
  init_audio();

  Serial.begin(9600);

  init_hexes();

  startupTest();
}

int fps = 15;
elapsedMillis drawTimer;
elapsedMillis idleTimer = 30000; // just so it starts up in idle mode
elapsedMillis patternSwitch;

// todo: move to patterns.h


PatternFunction curPattern;

void loop() {
  if (drawTimer > 1000 / fps) {
    drawTimer = 0;

    if (fft256_1.available()) {
      fadeAllLeds();
      float levels[8];
      LoadLevels(levels);

      if (patternSwitch / 1000 > PATTERN_TIMER) {
        FastLED.clear();
          idleIndex = random(5);
          patternIndex = random(7);
          Serial.print("New pattern: ");
          Serial.println(patternIndex + 1);
          Serial.print("New idle pattern: ");
          Serial.println(idleIndex + 1);
          patternSwitch = 0;
      }

      if (levels[0] > 1) {
        idleTimer = 0;
      }
      if (idleTimer / 1000 > 30) {
        idlePatterns[idleIndex]();
      }
      else {
        allPatterns[patternIndex](levels); 
      }
      FastLED.show();
    }
    else {
      //    Serial.println("fft not available???");
    }
  }
}

// fuck this
// was supposed to be a cool sine wave brightness thing
// looked like shit.
//int brightnessIndex = 0;
//int brightnesses[] = { 69, 72, 75, 79, 82, 86, 89, 93, 96, 100, 103, 106, 110, 113, 117, 120, 124, 127, 130, 134, 137, 140, 143, 146, 150, 152, 155, 158, 161, 164, 166, 169, 171, 174, 176, 178, 180, 178, 176, 174, 171, 169, 166, 164, 161, 158, 155, 152, 150, 146, 143, 140, 137, 134, 130, 127, 124, 120, 117, 113, 110, 106, 103, 100, 96, 93, 89, 86, 82, 79, 75, 72, 69, 65 };
//int brightlen = sizeof(brightnesses)/sizeof(int);
//void ringsTest4() {
//  for (int i = 0; i < 8; i++) {
//    int hue = 0;
//    brightnessIndex++;
//    if (brightnessIndex > brightlen) { brightnessIndex = 0; }
//    setRingColor(i, CHSV(hue, 255, brightnesses[brightnessIndex]));
//  }
//  delay(100);
//}
