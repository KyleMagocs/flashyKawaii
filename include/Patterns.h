#ifndef Patterns_h
#define Patterns_h


#include <SerialFlash.h>
#include <WS2812Serial.h>
#include "Hex.h"
#define USE_WS2812SERIAL  // the teensy 4 haaaaates you if you don't do this
#define FASTLED_INTERNAL

#define DECAYSPEED 1
#define DATA_PIN    14
#define NUM_LEDS    134
CRGBArray<NUM_LEDS> leds;
#define LED_TYPE    WS2812SERIAL
#define COLOR_ORDER BGR
#define BRIGHTNESS  120

#define BASS_THRESHOLD 10

int levelMax[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int startHues[8] = {0, 30, 60, 90, 120, 150, 180, 210};
int hueOffset = 0;

#pragma region UTILS
void init_leds(){
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
}

// move to util lib
void increaseHue(int amount) {
  hueOffset = (hueOffset + amount) % 255;
}

// just flashes all the lights white so I know it booted up.
void startupTest() {
  for (int i = 3; i < NUM_LEDS; i++) {
    FastLED.clear();
    leds[i] = CHSV(1, 0, 200);
    leds[i - 1] = CHSV(1, 0, 150);
    leds[i - 2] = CHSV(1, 0, 100);
    leds[i - 3] = CHSV(1, 0, 50);
    FastLED.show();
    delay(15);
  }
}

// todo: move to util lib
int huechangecounter = 0;  // we don't want to change every dang millisecond, yknow?
bool changeHueOnBass(float levels[]) {
  huechangecounter++;
  if (levels[0] > BASS_THRESHOLD && huechangecounter > 40) {
    increaseHue(80);
    huechangecounter = 0;
    return true;
  }
  return false;
}

void fadeAllLeds(){
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i].fadeToBlackBy(10);
  }
}

#pragma endregion

#pragma region REALPATTERNS

// spiral each hex out
void patternOne(float levels[]) {
  for (int i = 0; i < 7; i++)
  {
    int hue = (i * (255 / 7) + hueOffset);
    leds[hexes[6 - i].spiral[0]] = CHSV(hue, 255, 120); // keep the center lit, always
    int band_value = int(levels[i]);

    for (int j = 0; (j < band_value && j < 19) || j * DECAYSPEED < levelMax[i]; j++) {
      if ( j * DECAYSPEED > levelMax[i] ) {
        levelMax[i] = j * DECAYSPEED;
      }
      leds[hexes[6 - i].spiral[j]] = CHSV(hue, 255, 120);
    }

    if ( levelMax[i] > 6 ) { // why did I do this? why is it 6?
      leds[hexes[6 - i].spiral[int(levelMax[i] / DECAYSPEED)]] = CHSV(hue, 255, 180);
    }
    levelMax[i]--;
  }
}

// spiral each hex out, but rainbow!
void patternTwo(float levels[]) {
  increaseHue(2);
  patternOne(levels);
}

// this one does the dope rings for each band, and is the best one
void patternThree(float levels[]) {
  //  changeHueOnBass(levels);

  for (int i = 0; i < 8; i++) {
    levelMax[i]--;
    if (levelMax[i] < 0) levelMax[i] = 0;
    int hue = (i * (255 / 8) + hueOffset);
    int band_value = int(levels[i]);

    if ( band_value > 5) {
      levelMax[i] = 14;
      setRingColor(leds, i, CHSV(hue, 255, BRIGHTNESS) );
    }
    else {
      if (levelMax[i] > 0 && levelMax[i] <= 10) setRingColor(leds, i, CHSV(hue, 255, BRIGHTNESS) );
      if (levelMax[i] <= 8) setRingColor(leds, i, CHSV(hue, 255, BRIGHTNESS / 14 * levelMax[i]) );
    }
  }
}

// each hex is a band, and lights their rings outward like three
void patternFour(float levels[]) {

  for (int i = 0; i < 7; i++) {
    int hue = (i * (255 / 8) + hueOffset);
    int band_value = int(levels[i]);

    if ( band_value > 2) for (int j = 0; j < INNERLEN; j++) leds[hexes[6 - i].center[j]] = CHSV(hue, 255, BRIGHTNESS);
    if ( band_value > 5) for (int j = 0; j < MIDDLELEN; j++) leds[hexes[6 - i].middle[j]] = CHSV(hue + 40, 255, BRIGHTNESS);
    if ( band_value > 10) for (int j = 0; j < OUTERLEN; j++) leds[hexes[6 - i].outer[j]] = CHSV(hue + 80, 255, BRIGHTNESS);
  }
}

// each hex BUMPS and changes color on bass
void patternFive(float levels[]) {

  // this was used to flash everything white on color change
  // was kind of intense
//  int saturation = 255;
//  if (changeHueOnBass(levels)) {
//    saturation = 0;
//  }

  changeHueOnBass(levels);

  for (int i = 0; i < 7; i++)
  {
    int hue = (i * (255 / 7) + hueOffset);
    int band_value = int(levels[i]);

    for (int j = 0; j < 19; j++) {
      leds[hexes[6 - i].spiral[j]] = CHSV(hue, 255, (band_value + 5) * 7);
    }
  }
}

// each hex is a band, and lights up when the band crosses a threshold
void patternSix(float levels[]) {
  for (int i = 0; i < 7; i++)
  {
    int hue = (i * (255 / 7) + hueOffset);
    //    leds[hexes[6 - i].spiral[0]] = CHSV(hue, 255, 120); // keep the center lit, always
    int band_value = int(levels[i]);

    for (int j = 0; (j < band_value && j < 19) || j * DECAYSPEED < levelMax[i]; j++) {
      if ( j * DECAYSPEED > levelMax[i] ) {
        levelMax[i] = j * DECAYSPEED;
      }
      leds[hexes[6 - i].spiral[18 - j]] = CHSV(hue, 255, 120);
    }

    if ( levelMax[i] > 6 ) { // why did I do this? why is it 6?
      leds[hexes[6 - i].spiral[18 - int(levelMax[i] / DECAYSPEED)]] = CHSV(hue, 255, 180);
    }
    levelMax[i]--;
  }
}

// each hex is a band, and lights their rows outward
// but the center hex does its own thing
// kind of a combo of three and four
void patternSeven(float levels[]) {

  for (int i = 0; i < 6; i++) { // skip the center hex for now
    int band_value = int(levels[i]);


    if ( band_value * DECAYSPEED > levelMax[i] ) {
      levelMax[i] = band_value * DECAYSPEED;
    }
    // fade all the rings first, and then overwrite them with brighter colors if you need to
    // does this make sense?
    if ( levelMax[i]*DECAYSPEED > 2) for (int j = 0; j < 3; j++) leds[hexes[i].ring4[j]] = CHSV(85, 255, BRIGHTNESS / 18 * levelMax[i]);
    if ( levelMax[i]*DECAYSPEED > 3) for (int j = 0; j < 4; j++) leds[hexes[i].ring7[j]] = CHSV(85, 255, BRIGHTNESS / 18 * levelMax[i]);
    if ( levelMax[i]*DECAYSPEED > 5) for (int j = 0; j < 5; j++) leds[hexes[i].ring6[j]] = CHSV(48, 255, BRIGHTNESS / 18 * levelMax[i]);
    if ( levelMax[i]*DECAYSPEED > 8) for (int j = 0; j < 4; j++) leds[hexes[i].ring5[j]] = CHSV(48, 255, BRIGHTNESS / 18 * levelMax[i]);
    if ( levelMax[i]*DECAYSPEED > 10) for (int j = 0; j < 3; j++) leds[hexes[i].ring8[j]] = CHSV(0, 255, BRIGHTNESS / 18 * levelMax[i]);

    if ( band_value > 2) for (int j = 0; j < 3; j++) leds[hexes[i].ring4[j]] = CHSV(85, 255, BRIGHTNESS);
    if ( band_value > 3) for (int j = 0; j < 4; j++) leds[hexes[i].ring7[j]] = CHSV(85, 255, BRIGHTNESS);
    if ( band_value > 5) for (int j = 0; j < 5; j++) leds[hexes[i].ring6[j]] = CHSV(48, 255, BRIGHTNESS);
    if ( band_value > 8) for (int j = 0; j < 4; j++) leds[hexes[i].ring5[j]] = CHSV(48, 255, BRIGHTNESS);
    if ( band_value > 10) for (int j = 0; j < 3; j++) leds[hexes[i].ring8[j]] = CHSV(0, 255, BRIGHTNESS);
    levelMax[i]--;
  }
  // center hex
  int band_value = int(levels[0]);
  if ( band_value * DECAYSPEED > levelMax[0] ) {
    levelMax[0] = band_value * DECAYSPEED;
  }
  if ( levelMax[0] > 2) for (int j = 0; j < INNERLEN; j++) leds[hexes[6].center[j]] = CHSV(85, 255, BRIGHTNESS / 18 * levelMax[0]);
  if ( levelMax[0] > 5) for (int j = 0; j < MIDDLELEN; j++) leds[hexes[6].middle[j]] = CHSV(85, 255, BRIGHTNESS / 18 * levelMax[0]);
  if ( levelMax[0] > 10) for (int j = 0; j < OUTERLEN; j++) leds[hexes[6].outer[j]] = CHSV(85, 255, BRIGHTNESS / 18 * levelMax[0]);
  
  if ( band_value > 2) for (int j = 0; j < INNERLEN; j++) leds[hexes[6].center[j]] = CHSV(85, 255, BRIGHTNESS);
  if ( band_value > 5) for (int j = 0; j < MIDDLELEN; j++) leds[hexes[6].middle[j]] = CHSV(85, 255, BRIGHTNESS);
  if ( band_value > 10) for (int j = 0; j < OUTERLEN; j++) leds[hexes[6].outer[j]] = CHSV(85, 255, BRIGHTNESS);
}

//inverted seven
void patternEight(float levels[]) {

  for (int i = 0; i < 6; i++) { // skip the center hex for now
    int band_value = int(levels[i]);


    if ( band_value * DECAYSPEED > levelMax[i] ) {
      levelMax[i] = band_value * DECAYSPEED;
    }
    // fade all the rings first, and then overwrite them with brighter colors if you need to
    // does this make sense?
    if ( levelMax[i]*DECAYSPEED > 2) for (int j = 0; j < 3; j++) leds[hexes[i].ring8[j]] = CHSV(85, 255, BRIGHTNESS / 18 * levelMax[i]);
    if ( levelMax[i]*DECAYSPEED > 3) for (int j = 0; j < 4; j++) leds[hexes[i].ring5[j]] = CHSV(85, 255, BRIGHTNESS / 18 * levelMax[i]);
    if ( levelMax[i]*DECAYSPEED > 5) for (int j = 0; j < 5; j++) leds[hexes[i].ring6[j]] = CHSV(48, 255, BRIGHTNESS / 18 * levelMax[i]);
    if ( levelMax[i]*DECAYSPEED > 8) for (int j = 0; j < 4; j++) leds[hexes[i].ring7[j]] = CHSV(48, 255, BRIGHTNESS / 18 * levelMax[i]);
    if ( levelMax[i]*DECAYSPEED > 10) for (int j = 0; j < 3; j++) leds[hexes[i].ring4[j]] = CHSV(0, 255, BRIGHTNESS / 18 * levelMax[i]);

    if ( band_value > 2) for (int j = 0; j < 3; j++) leds[hexes[i].ring8[j]] = CHSV(85, 255, BRIGHTNESS);
    if ( band_value > 3) for (int j = 0; j < 4; j++) leds[hexes[i].ring5[j]] = CHSV(85, 255, BRIGHTNESS);
    if ( band_value > 5) for (int j = 0; j < 5; j++) leds[hexes[i].ring6[j]] = CHSV(48, 255, BRIGHTNESS);
    if ( band_value > 8) for (int j = 0; j < 4; j++) leds[hexes[i].ring7[j]] = CHSV(48, 255, BRIGHTNESS);
    if ( band_value > 10) for (int j = 0; j < 3; j++) leds[hexes[i].ring4[j]] = CHSV(0, 255, BRIGHTNESS);
    levelMax[i]--;
  }
  // center hex
  int band_value = int(levels[0]);
  if ( band_value * DECAYSPEED > levelMax[0] ) {
    levelMax[0] = band_value * DECAYSPEED;
  }
  if ( levelMax[0] > 2) for (int j = 0; j < INNERLEN; j++) leds[hexes[6].center[j]] = CHSV(85, 0, BRIGHTNESS / 18 * levelMax[0]);
  if ( levelMax[0] > 5) for (int j = 0; j < MIDDLELEN; j++) leds[hexes[6].middle[j]] = CHSV(85, 0, BRIGHTNESS / 18 * levelMax[0]);
  if ( levelMax[0] > 10) for (int j = 0; j < OUTERLEN; j++) leds[hexes[6].outer[j]] = CHSV(85, 0, BRIGHTNESS / 18 * levelMax[0]);
  
  if ( band_value > 2) for (int j = 0; j < INNERLEN; j++) leds[hexes[6].center[j]] = CHSV(85, 0, BRIGHTNESS);
  if ( band_value > 5) for (int j = 0; j < MIDDLELEN; j++) leds[hexes[6].middle[j]] = CHSV(85, 0, BRIGHTNESS);
  if ( band_value > 10) for (int j = 0; j < OUTERLEN; j++) leds[hexes[6].outer[j]] = CHSV(85, 0, BRIGHTNESS);
}

#pragma endregion

#pragma region idle patterns
//each hex spirals out, and then spirals back in
void IdlePatternOne() {
  int saturation = 0;

  for (int i = 0; i < 19; i++) {
    if (i > 0) saturation = 100;
    if (i > 6) saturation = 255;
    // saturation += 13;
    for (int j = 0; j < NUMHEXES; j++) {
      leds[hexes[j].spiral[i]] = CHSV(255 / 7 * j + hueOffset, saturation, BRIGHTNESS);
    }
    FastLED.show();
    delay(100);
  }
  for (int i = 18; i >= 0; i--) {
    for (int j = 0; j < NUMHEXES; j++) {
      leds[hexes[j].spiral[i]] = CHSV(0, 0, 0);
    }
    FastLED.show();
    delay(100);
  }
  increaseHue(36);
}

// rainbow shifting across rings
void IdlePatternTwo() {
  for (int i = 0; i < 8; i++) {
    int hue = (i * (255 / 8) + hueOffset);
    setRingColor(leds, i, CHSV(hue, 255, BRIGHTNESS));
  }
  increaseHue(4);
}

// rainbow across each hex's rings
void IdlePatternThree() {
  for (int j = 0; j < INNERLEN; j++) {
    for (int i = 0; i < NUMHEXES; i++) {
      leds[hexes[i].center[j]] = CHSV(0 + 255/7*i + hueOffset, 255, BRIGHTNESS);
    }
  }
  for (int j = 0; j < MIDDLELEN; j++) {
    for (int i = 0; i < NUMHEXES; i++) {
      leds[hexes[i].middle[j]] = CHSV(85  + 255/7*i + hueOffset, 255, BRIGHTNESS);
    }
  }
  for (int j = 0; j < OUTERLEN; j++) {
    for (int i = 0; i < NUMHEXES; i++) {
      leds[hexes[i].outer[j]] = CHSV(170  + 255/7*i + hueOffset, 255, BRIGHTNESS);
    }
  }
  increaseHue(3);
}

// sets the outline of the board and the inner hex to shifting colors.  
// I dunno it looks cool.
void IdlePatternFour() {
  for (int i = 0; i < 42; i++) {
    leds[outline[i]] = CHSV(hueOffset + (255 / 42 * i), 255, BRIGHTNESS);
  }
  for (int j = 0; j < 12; j++) {
    leds[hexes[6].ring3[j]] = CHSV(hueOffset + (255 / 12 * (j+2)), 255, BRIGHTNESS);
  }
  increaseHue(3);
}

//each hex spirals out, and then spirals off out
void IdlePatternFive() {
  int saturation = 0;

  for (int i = 0; i < 19; i++) {
    saturation += 13;
    for (int j = 0; j < NUMHEXES; j++) {
      leds[hexes[j].spiral[i]] = CHSV(255 / 7 * j + hueOffset, saturation, BRIGHTNESS);
    }
    FastLED.show();
    delay(100);
  }
  for (int i = 0; i < 19; i++) {
    for (int j = 0; j < NUMHEXES; j++) {
      leds[hexes[j].spiral[i]] = CHSV(0, 0, 0);
    }
    FastLED.show();
    delay(100);
  }
  increaseHue(36);
}
#pragma endregion

// test trash, don't use
// or maybe re-visit, I dunno.
int six_counter = 0;
void IdlePatternSix() {
  leds[hexes[6].ring3[six_counter]] = CHSV(0, 0, BRIGHTNESS);
  six_counter = (six_counter+1)%12;
}
#endif
