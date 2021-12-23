#ifndef Patterns_h
#define Patterns_h

#include <SerialFlash.h>
#include <WS2812Serial.h>
#include "Hex.h"
#include "Palette.h"
#include "AudioControl.h"
#include "constants.h"


#define USE_WS2812SERIAL // the teensy 4 haaaaates you if you don't do this.  If you're reading this and using a teensy 3, probably remove it??
#define FASTLED_INTERNAL
#define DATA_PIN 14
#define LED_TYPE WS2812SERIAL
#define COLOR_ORDER BGR

CRGBArray<NUM_LEDS_FULL> leds;

int decayFactor = DECAY;
// int levelMax[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int idleStartHues[8] = {0, 30, 60, 90, 120, 150, 180, 210};
int hueOffset = 0;

#pragma region UTILS
void init_leds()
{
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS_FULL);
  // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(secret_leds, 150);
}

// move to util lib
void increaseHue(int amount)
{
  hueOffset = (hueOffset + amount) % 255;
}

int wheelIndex = 0;
void increaseWheel(int amount)
{
  wheelIndex = (wheelIndex + amount) % 255;
}

elapsedMillis bassTimer = 0;
elapsedMillis bpmTimer = 0;
float lastHit = 0;
float estBpm = 100.0; // start at 200 and work down or up from there
// int basscounter = 0; // we don't want to change every dang millisecond, yknow?
bool bassHit(float levels[])
{
  // return getLowBeat();
  // basscounter++;

  bool bassReady =  float(bassTimer) > float( 60 / estBpm * 1000); // 60s / x bpm * millis)
  int lowend = int((levels[0] + levels[1])/2);
  bool thresholdHit = lowend > BASS_THRESHOLD; 

  // well this was a mess.
  // if (lowend < 2) return;

  
  // Serial.printf("Timer:%2f ", float(bassTimer));
  // Serial.printf("BpmTimer:%d ", bpmTimer/1000);
  // Serial.printf("Lowend:%d ", lowend / BASS_THRESHOLD);
  // Serial.printf("LastHit:%f ", lastHit);
  // Serial.printf("Threshold:%d ", thresholdHit);
  // Serial.printf("Ready:%d ", bassReady);
  // Serial.printf("bpm:%d ", int(estBpm));

  // if (thresholdHit){
  //   estBpm = (estBpm + (lastHit - bpmTimer)) / 2; // time since last hit * 1000ms * 60s
  //   lastHit = float(bpmTimer);
  //   bpmTimer = 0;
  // }
  // if (bassReady && thresholdHit) { 
  //   bassTimer = 0;
  // }
  
  // Serial.println("");

  return thresholdHit && bassReady;
  // if ((levels[0] + levels[1] / 2) > BASS_THRESHOLD)
  // {
  //   // Serial.printf("Bass Counter: %d\n", huechangecounter);
  //   if (basscounter > 2)
  //   { // yes, this could have been a &&, but separating the ifs helps debug
  //     basscounter = 0;
  //     // Serial.println((levels[0] + levels[1] / 2));
  //     // increaseHue(20);
  //     return true;
  //   }
  // }
  // return false;
}

void fadeAllLeds()
{
  for (int i = 0; i < NUM_LEDS_FULL; i++)
  {
    leds[i].fadeToBlackBy(decayFactor);
  }
}

void drawWheelWithPalette(int value, CRGBPalette16 palette)
{
  for (int i = 0; i < 42; i++)
  {
    // leds[outline[i]] = CHSV(hueOffset + (255 / 42 * i*num_rainbows), 255, value);
    leds[outline[i]] = ColorFromPalette(palette, scale8(255 / 42 * i + wheelIndex, 240), BRIGHTNESS, LINEARBLEND);
  }
  for (int i = 0; i < 36; i++)
  {
    // leds[outline2[i]] = CHSV(hueOffset + (255 / 36 * i*num_rainbows), 255, value);
    leds[outline2[i]] = ColorFromPalette(palette, scale8(255 / 36 * i + wheelIndex, 240), BRIGHTNESS, LINEARBLEND);
  }
  for (int i = 0; i < 30; i++)
  {
    leds[outline3[i]] = ColorFromPalette(palette, scale8(255 / 30 * i + wheelIndex, 240), BRIGHTNESS, LINEARBLEND);
    // leds[outline3[i]] = CHSV(hueOffset + (255 / 30 * i*num_rainbows), 255, value);
  }
  for (int i = 0; i < 6; i++)
  {
    leds[outline4[i]] = ColorFromPalette(palette, scale8(255 / 6 * i + wheelIndex, 240), BRIGHTNESS, LINEARBLEND);
    // leds[outline4[i]] = CHSV(hueOffset + (255 / 6 * i*num_rainbows), 255, value);
  }
  wheelIndex++;
  wheelIndex %= 255;
  // for (int j = 0; j < 12; j++)
  // {
  //   leds[hexes[6].ring3[j]] = CHSV(hueOffset + (255 / 12 * (j+1)), 255, value);
  // }
  // for (int j = 0; j < 6; j++)
  // {
  //   leds[hexes[6].ring2[j]] = CHSV(hueOffset + (255 / 6 * (j+1)), 255, value);
  // }
  // increaseHue(3);
}

void drawRainbowWheel(int value)
{
  drawWheelWithPalette(value, RainbowColors_p);
}

// just flashes all the lights white so I know it booted up.
void startupTest()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    fadeAllLeds();
    leds[i] = CHSV(1, 0, 200);
    FastLED.show();
    delay(15);
  }
}


// fastLED does something to light up lights past the 133 so like, let's just force those off, I guess!
void killTheExtras(){
  for(int i = 134; i < 150; i++){
    Serial.printf("Clearing led: %d ", i);
    leds[i] = CHSV(0,0,0);
  }
}
#pragma endregion

#pragma region REALPATTERNS

// spiral each hex out
void p_spiralout(float levels[])
{
  for (int i = 0; i < 7; i++)
  {
    int hue = (i * (255 / 7) + hueOffset);
    leds[hexes[6 - i].spiral[0]] = CHSV(hue, 255, 120); // keep the center lit, always
    int band_value = int(levels[i]);

    for (int j = 0; (j < band_value && j < 19); j++)
    {
      leds[hexes[6 - i].spiral[j]] = CHSV(hue, 255, 120);
    }
  }
}

// spiral each hex out, but rainbow!
void p_spiralout_rainbow(float levels[])
{
  increaseHue(2);
  p_spiralout(levels);
}

// this one does the dope rings for each band, and is the best one
void p_ringBands(float levels[])
{
  for (int i = 0; i < 8; i++)
  {
    int hue = (i * (255 / 8) + hueOffset);
    int band_value = int(levels[i]);

    if (band_value > 9)  // this is arbitrary but it feels good.
    {
      int x = i;
      if (i == 4) x = 6; // I did this by mistake early on and it turns out it makes it way better
      if (i == 6) x = 4;
      if (i == 2) x = 0;
      if (i == 0) x = 2;
      setRingColor(leds, x, CHSV(hue, 255, BRIGHTNESS));
    }
  }
}

// it's pattern three.  but with the rings sorta inverted.  Not actually inverted.  Winging it here.  Cut me some slack.
void p_ringBands_inverted(float levels[])
{
  for (int i = 0; i < 8; i++)
  {
    int hue = (i * (255 / 8) + hueOffset);
    int band_value = int(levels[i]);

    if (band_value > 9)  // this is arbitrary but it feels good.
    {
      int x = i;
      
      if (i == 0) x = 5;
      if (i == 1) x = 4;
      if (i == 2) x = 3;
      if (i == 3) x = 2;
      if (i == 4) x = 1;
      if (i == 5) x = 6;
      if (i == 6) x = 7;
      if (i == 7) x = 0;

      setRingColor(leds, x, CHSV(hue, 255, BRIGHTNESS));
    }
  }
}

// each hex is a band, and lights their rings outward like three
void p_hexBands(float levels[])
{
  for (int i = 0; i < 7; i++)
  {
    int hue = (i * (255 / 8) + hueOffset);
    int band_value = int(levels[i]);

    if (band_value > 2)
      for (int j = 0; j < INNERLEN; j++)
        leds[hexes[6 - i].center[j]] = CHSV(hue, 255, BRIGHTNESS);
    if (band_value > 6)
      for (int j = 0; j < MIDDLELEN; j++)
        leds[hexes[6 - i].middle[j]] = CHSV(hue + 20, 255, BRIGHTNESS);
    if (band_value > 12)
      for (int j = 0; j < OUTERLEN; j++)
        leds[hexes[6 - i].outer[j]] = CHSV(hue + 40, 255, BRIGHTNESS);
  }
}

// each hex BUMPS
// I think this sucks and is actually just a dupe of four.  I forget what I was going for.
void p_butts_sucks_dont_use_me(float levels[])
{
  bassHit(levels);

  for (int i = 0; i < 7; i++)
  {
    int hue = (i * 20 + hueOffset);
    int band_value = levels[i];

    if (band_value > 2)
      for (int j = 0; j < INNERLEN; j++)
        leds[hexes[6 - i].center[j]] = CHSV(hue, 255, BRIGHTNESS);
    if (band_value > 6)
      for (int j = 0; j < MIDDLELEN; j++)
        leds[hexes[6 - i].middle[j]] = CHSV(hue, 255, BRIGHTNESS);
    if (band_value > 12)
      for (int j = 0; j < OUTERLEN; j++)
        leds[hexes[6 - i].outer[j]] = CHSV(hue, 255, BRIGHTNESS);
  }

  // for (int j = 0; j < 19; j++)
  // {
  //   leds[hexes[6 - i].spiral[j]] = CHSV(hue, 255, min(band_value * 8, BRIGHTNESS)); // this is ugly and I am dumb
  // }
}

// each hex is a band, and spirals inward
void p_spiralIn(float levels[])
{
  for (int i = 0; i < 7; i++)
  {
    int hue = (i * 20 + hueOffset);
    //    leds[hexes[6 - i].spiral[0]] = CHSV(hue, 255, 120); // keep the center lit, always
    int band_value = int(levels[i]);

    for (int j = 0; (j < band_value && j < 19); j++)
    {
      leds[hexes[6 - i].spiral[18 - j]] = CHSV(hue, 255, BRIGHTNESS);
    }
  }
}

// each hex is a band, and lights their rows outward
// but the center hex does its own thing
// kind of a combo of three and four
void p_vuMeter(float levels[])
{
  decayFactor = DECAY/1.5;
  for (int i = 0; i < 6; i++)
  { // skip the center hex for now
    int band_value = int(levels[i]);

    if (band_value > 3)
      for (int j = 0; j < 3; j++)
        leds[hexes[i].ring4[j]] = CHSV(hueOffset + 85, 255, BRIGHTNESS);
    if (band_value > 5)
      for (int j = 0; j < 4; j++)
        leds[hexes[i].ring5[j]] = CHSV(hueOffset + 85, 255, BRIGHTNESS);
    if (band_value > 8)
      for (int j = 0; j < 5; j++)
        leds[hexes[i].ring6[j]] = CHSV(hueOffset + 48, 255, BRIGHTNESS);
    if (band_value > 11)
      for (int j = 0; j < 4; j++)
        leds[hexes[i].ring7[j]] = CHSV(hueOffset + 48, 255, BRIGHTNESS);
    if (band_value > 14)
      for (int j = 0; j < 3; j++)
        leds[hexes[i].ring8[j]] = CHSV(hueOffset + 0, 255, BRIGHTNESS);
  }
  // center hex
  int band_value = int(levels[0]);
  if (band_value > 3)
    for (int j = 0; j < INNERLEN; j++)
      leds[hexes[6].center[j]] = CHSV(hueOffset + 85, 255, BRIGHTNESS);

  if (band_value > 8)
    for (int j = 0; j < MIDDLELEN; j++)
      leds[hexes[6].middle[j]] = CHSV(hueOffset + 85, 255, BRIGHTNESS);

  if (band_value > 13)
    for (int j = 0; j < OUTERLEN; j++)
      leds[hexes[6].outer[j]] = CHSV(hueOffset + 85, 255, BRIGHTNESS);
}

//inverted seven
void p_vuMeter_inverted(float levels[])
{
  decayFactor = DECAY/1.5;
  for (int i = 0; i < 6; i++)
  { // skip the center hex for now
    int band_value = int(levels[i]);

    if (band_value > 3)
      for (int j = 0; j < 3; j++)
        leds[hexes[i].ring8[j]] = CHSV(hueOffset + 85, 255, BRIGHTNESS);
    if (band_value > 5)
      for (int j = 0; j < 4; j++)
        leds[hexes[i].ring7[j]] = CHSV(hueOffset + 85, 255, BRIGHTNESS);
    if (band_value > 8)
      for (int j = 0; j < 5; j++)
        leds[hexes[i].ring6[j]] = CHSV(hueOffset + 48, 255, BRIGHTNESS);
    if (band_value > 11)
      for (int j = 0; j < 4; j++)
        leds[hexes[i].ring5[j]] = CHSV(hueOffset + 48, 255, BRIGHTNESS);
    if (band_value > 14)
      for (int j = 0; j < 3; j++)
        leds[hexes[i].ring4[j]] = CHSV(hueOffset + 0, 255, BRIGHTNESS);
  }
  // center hex
  int band_value = int(levels[0]);
  if (band_value > 3)
    for (int j = 0; j < INNERLEN; j++)
      leds[hexes[6].center[j]] = CHSV(85, 0, BRIGHTNESS);
  if (band_value > 8)
    for (int j = 0; j < MIDDLELEN; j++)
      leds[hexes[6].middle[j]] = CHSV(85, 0, BRIGHTNESS);
  if (band_value > 13)
    for (int j = 0; j < OUTERLEN; j++)
      leds[hexes[6].outer[j]] = CHSV(85, 0, BRIGHTNESS);
}

// okay just hear me out on this one
void p_spinTheWheel(float levels[])
{

  int band_value = int(levels[0]);
  if (band_value > 2)
    for (int j = 0; j < INNERLEN; j++)
      leds[hexes[6].center[j]] = CHSV(85, 0, BRIGHTNESS);
  if (band_value > 7)
    for (int j = 0; j < MIDDLELEN; j++)
      leds[hexes[6].middle[j]] = CHSV(85, 0, BRIGHTNESS);
  if (band_value > BASS_THRESHOLD)
    for (int j = 0; j < OUTERLEN; j++)
      leds[hexes[6].outer[j]] = CHSV(85, 0, BRIGHTNESS);

  if (bassHit(levels))
  {
    for (int i = 0; i < 12; i++)
    {
      increaseWheel(1);
      drawWheelWithPalette(BRIGHTNESS, getCurrentPalette());
      FastLED.show();
      delay(.1);
    }
  }
  else
  {
    increaseWheel(.5);
    drawWheelWithPalette(BRIGHTNESS, getCurrentPalette());
  }
}

// it's like nine but a bit more bass-important?
void p_fadeWheel(float levels[])
{
  decayFactor = DECAY / 2;
  int band_value = int(levels[0]);
  if (band_value > 2)
    for (int j = 0; j < INNERLEN; j++)
      leds[hexes[6].center[j]] = CHSV(85, 0, BRIGHTNESS);
  if (band_value > 7)
    for (int j = 0; j < MIDDLELEN; j++)
      leds[hexes[6].middle[j]] = CHSV(85, 0, BRIGHTNESS);
  if (band_value > BASS_THRESHOLD)
    for (int j = 0; j < OUTERLEN; j++)
      leds[hexes[6].outer[j]] = CHSV(85, 0, BRIGHTNESS);

  if (bassHit(levels))
  {
    for (int i = 0; i < 20; i++)
    {
      increaseWheel(.5);
      drawWheelWithPalette(BRIGHTNESS, getCurrentPalette());
      FastLED.show();
      delay(.25);
    }
  }
  else
  {  }
}
#pragma endregion

#endif