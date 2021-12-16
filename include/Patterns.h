#ifndef Patterns_h
#define Patterns_h

#include <SerialFlash.h>
#include <WS2812Serial.h>
#include "Hex.h"
#define USE_WS2812SERIAL // the teensy 4 haaaaates you if you don't do this
#define FASTLED_INTERNAL

#define DECAYSPEED 1
#define DATA_PIN 14
#define NUM_LEDS 134
CRGBArray<NUM_LEDS> leds;
#define LED_TYPE WS2812SERIAL
#define COLOR_ORDER BGR
#define BRIGHTNESS 200

// int levelMax[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int startHues[8] = {0, 30, 60, 90, 120, 150, 180, 210};
int hueOffset = 0;

#pragma region palettes
CRGB purple = CHSV(HUE_PURPLE, 255, 255);
CRGB green = CHSV(HUE_GREEN, 255, 255);
CRGB black = CRGB::Black;
CRGB white = CRGB::White;

CRGBPalette16 GreenAndPurple_p = CRGBPalette16(
    green, green, black, black,
    purple, purple, black, black,
    green, green, black, black,
    purple, purple, black, black);

CRGBPalette16 WhiteStripes_p = CRGBPalette16(
    CRGB::Red, black, black, black,
    CRGB::Blue, black, black, black,
    CRGB::Green, black, black, black,
    CRGB::Blue, black, black, black);

// Gradient palette "bhw2_50_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_50.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw2_50_gp ) {
    0,   8,  2, 23,
   84,  47,  7,102,
  138,  16, 46,147,
  173,   2,127,203,
  255,   1,  7, 11};

#pragma endregion

#pragma region UTILS
void init_leds()
{
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
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

#define BASS_THRESHOLD 20
int basscounter = 0; // we don't want to change every dang millisecond, yknow?
bool bassHit(float levels[])
{
  basscounter++;
  if (levels[0] > BASS_THRESHOLD)
  {
    // Serial.printf("Bass Counter: %d\n", huechangecounter);
    if (basscounter > 2)
    { // yes, this could have been a &&, but separating the ifs helps debug
      basscounter = 0;
      Serial.println(levels[0]);
      // increaseHue(20);
      return true;
    }
  }
  return false;
}

void fadeAllLeds()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].fadeToBlackBy(90);
  }
}

void drawWheelWithPalette(int value, CRGBPalette16 palette)
{
  for (int i = 0; i < 42; i++)
  {
    // leds[outline[i]] = CHSV(hueOffset + (255 / 42 * i*num_rainbows), 255, value);
    leds[outline[i]] = ColorFromPalette(palette, scale8( 255 / 42 * i + wheelIndex, 240), BRIGHTNESS, LINEARBLEND);
  }
  for (int i = 0; i < 36; i++)
  {
    // leds[outline2[i]] = CHSV(hueOffset + (255 / 36 * i*num_rainbows), 255, value);
    leds[outline2[i]] = ColorFromPalette(palette, scale8( 255 / 36 * i + wheelIndex, 240), BRIGHTNESS, LINEARBLEND);
  }
  for (int i = 0; i < 30; i++)
  {
    leds[outline3[i]] = ColorFromPalette(palette, scale8( 255 / 30 * i + wheelIndex, 240), BRIGHTNESS, LINEARBLEND);
    // leds[outline3[i]] = CHSV(hueOffset + (255 / 30 * i*num_rainbows), 255, value);
  }
  for (int i = 0; i < 6; i++)
  {
    leds[outline4[i]] = ColorFromPalette(palette, scale8( 255 / 6 * i + wheelIndex, 240), BRIGHTNESS, LINEARBLEND);
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

#pragma endregion

#pragma region REALPATTERNS

// spiral each hex out
void patternOne(float levels[])
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
void patternTwo(float levels[])
{
  increaseHue(2);
  patternOne(levels);
}

// this one does the dope rings for each band, and is the best one
void patternThree(float levels[])
{
  //  changeHueOnBass(levels);

  for (int i = 0; i < 8; i++)
  {
    int hue = (i * (255 / 8) + hueOffset);
    int band_value = int(levels[i]);

    if (band_value > 5)
    {
      setRingColor(leds, i, CHSV(hue, 255, BRIGHTNESS));
    }
  }
}

// each hex is a band, and lights their rings outward like three
void patternFour(float levels[])
{

  for (int i = 0; i < 7; i++)
  {
    int hue = (i * (255 / 8) + hueOffset);
    int band_value = int(levels[i]);

    if (band_value > 2)
      for (int j = 0; j < INNERLEN; j++)
        leds[hexes[6 - i].center[j]] = CHSV(hue, 255, BRIGHTNESS);
    if (band_value > 5)
      for (int j = 0; j < MIDDLELEN; j++)
        leds[hexes[6 - i].middle[j]] = CHSV(hue + 20, 255, BRIGHTNESS);
    if (band_value > 10)
      for (int j = 0; j < OUTERLEN; j++)
        leds[hexes[6 - i].outer[j]] = CHSV(hue + 40, 255, BRIGHTNESS);
  }
}

// each hex BUMPS and changes color on bass
void patternFive(float levels[])
{
  bassHit(levels);

  for (int i = 0; i < 7; i++)
  {
    int hue = (i * 20 + hueOffset);
    int band_value = levels[i];

    for (int j = 0; j < 19; j++)
    {
      leds[hexes[6 - i].spiral[j]] = CHSV(hue, 255, 50);                                       // ensure a minimum brightness
      leds[hexes[6 - i].spiral[j]] = CHSV(hue, 255, min(max(50, band_value * 8), BRIGHTNESS)); // this is ugly and I am dumb
    }
  }
}

// each hex is a band, and spirals inward
void patternSix(float levels[])
{
  for (int i = 0; i < 7; i++)
  {
    int hue = (i * 20 + hueOffset);
    //    leds[hexes[6 - i].spiral[0]] = CHSV(hue, 255, 120); // keep the center lit, always
    int band_value = int(levels[i]);

    for (int j = 0; (j < band_value && j < 19); j++)
    {
      leds[hexes[6 - i].spiral[18 - j]] = CHSV(hue, 255, 120);
    }
  }
}

// each hex is a band, and lights their rows outward
// but the center hex does its own thing
// kind of a combo of three and four
void patternSeven(float levels[])
{

  for (int i = 0; i < 6; i++)
  { // skip the center hex for now
    int band_value = int(levels[i]);

    if (band_value > 2)
      for (int j = 0; j < 3; j++)
        leds[hexes[i].ring4[j]] = CHSV(hueOffset + 85, 255, BRIGHTNESS);
    if (band_value > 3)
      for (int j = 0; j < 4; j++)
        leds[hexes[i].ring5[j]] = CHSV(hueOffset + 85, 255, BRIGHTNESS);
    if (band_value > 5)
      for (int j = 0; j < 5; j++)
        leds[hexes[i].ring6[j]] = CHSV(hueOffset + 48, 255, BRIGHTNESS);
    if (band_value > 8)
      for (int j = 0; j < 4; j++)
        leds[hexes[i].ring7[j]] = CHSV(hueOffset + 48, 255, BRIGHTNESS);
    if (band_value > 10)
      for (int j = 0; j < 3; j++)
        leds[hexes[i].ring8[j]] = CHSV(hueOffset + 0, 255, BRIGHTNESS);
  }
  // center hex
  int band_value = int(levels[0]);
  if (band_value > 2)
    for (int j = 0; j < INNERLEN; j++)
      leds[hexes[6].center[j]] = CHSV(hueOffset + 85, 255, BRIGHTNESS);
  if (band_value > 5)
    for (int j = 0; j < MIDDLELEN; j++)
      leds[hexes[6].middle[j]] = CHSV(hueOffset + 85, 255, BRIGHTNESS);
  if (band_value > 10)
    for (int j = 0; j < OUTERLEN; j++)
      leds[hexes[6].outer[j]] = CHSV(hueOffset + 85, 255, BRIGHTNESS);
}

//inverted seven
void patternEight(float levels[])
{

  for (int i = 0; i < 6; i++)
  { // skip the center hex for now
    int band_value = int(levels[i]);

    if (band_value > 2)
      for (int j = 0; j < 3; j++)
        leds[hexes[i].ring8[j]] = CHSV(hueOffset + 85, 255, BRIGHTNESS);
    if (band_value > 3)
      for (int j = 0; j < 4; j++)
        leds[hexes[i].ring7[j]] = CHSV(hueOffset + 85, 255, BRIGHTNESS);
    if (band_value > 5)
      for (int j = 0; j < 5; j++)
        leds[hexes[i].ring6[j]] = CHSV(hueOffset + 48, 255, BRIGHTNESS);
    if (band_value > 8)
      for (int j = 0; j < 4; j++)
        leds[hexes[i].ring5[j]] = CHSV(hueOffset + 48, 255, BRIGHTNESS);
    if (band_value > 10)
      for (int j = 0; j < 3; j++)
        leds[hexes[i].ring4[j]] = CHSV(hueOffset + 0, 255, BRIGHTNESS);
  }
  // center hex
  int band_value = int(levels[0]);
  if (band_value > 2)
    for (int j = 0; j < INNERLEN; j++)
      leds[hexes[6].center[j]] = CHSV(85, 0, BRIGHTNESS);
  if (band_value > 7)
    for (int j = 0; j < MIDDLELEN; j++)
      leds[hexes[6].middle[j]] = CHSV(85, 0, BRIGHTNESS);
  if (band_value > 12)
    for (int j = 0; j < OUTERLEN; j++)
      leds[hexes[6].outer[j]] = CHSV(85, 0, BRIGHTNESS);
}

// okay just hear me out on this one
void patternNine(float levels[])
{

  int band_value = int(levels[0]);
  if (band_value > 2)
    for (int j = 0; j < INNERLEN; j++)
      leds[hexes[6].center[j]] = CHSV(85, 0, BRIGHTNESS);
  if (band_value > 7)
    for (int j = 0; j < MIDDLELEN; j++)
      leds[hexes[6].middle[j]] = CHSV(85, 0, BRIGHTNESS);
  if (band_value > 12)
    for (int j = 0; j < OUTERLEN; j++)
      leds[hexes[6].outer[j]] = CHSV(85, 0, BRIGHTNESS);

  if (bassHit(levels))
  {
    for (int i = 0; i < 20; i++)
    {
      increaseWheel(1);
      drawWheelWithPalette(BRIGHTNESS, bhw2_50_gp);
      FastLED.show();
      delay(.2);
    }
  }
  else
  {
    increaseWheel(.5);
    drawWheelWithPalette(BRIGHTNESS, bhw2_50_gp);
  }
}
#pragma endregion

#pragma region idle patterns
//each hex spirals out, and then spirals back in
void IdlePatternOne()
{
  int saturation = 0;

  for (int i = 0; i < 19; i++)
  {
    if (i > 0)
      saturation = 100;
    if (i > 6)
      saturation = 255;
    // saturation += 13;
    for (int j = 0; j < NUMHEXES; j++)
    {
      leds[hexes[j].spiral[i]] = CHSV(255 / 7 * j + hueOffset, saturation, 100);
    }
    FastLED.show();
    delay(100);
  }
  for (int i = 18; i >= 1; i--)
  {
    for (int j = 0; j < NUMHEXES; j++)
    {
      leds[hexes[j].spiral[i]] = CHSV(0, 0, 0);
    }
    FastLED.show();
    delay(100);
  }
  increaseHue(36);
}

//each hex spirals out.
int spiralIndex = 0;
void IdlePatternOne_better()
{
  int saturation = 8 * spiralIndex;
  for (int j = 0; j < NUMHEXES; j++)
  {
    leds[hexes[j].spiral[0]] = CHSV(255 / 7 * j + hueOffset, saturation - 10, 90);
    leds[hexes[j].spiral[spiralIndex]] = CHSV(255 / 7 * j + hueOffset, saturation, 100);
  }
  delay(100);

  increaseHue(5);
  spiralIndex %= 19;
  spiralIndex++;
}

// rainbow shifting across rings
void IdlePatternTwo()
{
  for (int i = 0; i < 8; i++)
  {
    int hue = (i * (255 / 8) + hueOffset);
    setRingColor(leds, i, CHSV(hue, 255, 100));
  }
  increaseHue(4);
}

// rainbow across each hex's rings
void IdlePatternThree()
{
  for (int j = 0; j < INNERLEN; j++)
  {
    for (int i = 0; i < NUMHEXES; i++)
    {
      leds[hexes[i].center[j]] = CHSV(0 + 255 / 9 * i * .8 + hueOffset, 255, 100);
    }
  }
  for (int j = 0; j < MIDDLELEN; j++)
  {
    for (int i = 0; i < NUMHEXES; i++)
    {
      leds[hexes[i].middle[j]] = CHSV(20 + 255 / 9 * i * .8 + hueOffset, 255, 100);
    }
  }
  for (int j = 0; j < OUTERLEN; j++)
  {
    for (int i = 0; i < NUMHEXES; i++)
    {
      leds[hexes[i].outer[j]] = CHSV(40 + 255 / 9 * i * .8 + hueOffset, 255, 100);
    }
  }
  increaseHue(1);
}

// sets the outline of the board and the inner hex to shifting colors.
// I dunno it looks cool.
void IdlePatternFour()
{
  for (int i = 0; i < 42; i++)
  {
    leds[outline[i]] = CHSV(hueOffset + (255 / 42 * i), 255, 100);
  }
  for (int j = 0; j < 12; j++)
  {
    leds[hexes[6].ring3[j]] = CHSV(hueOffset + (255 / 12 * (j + 2)), 255, 100);
  }
  increaseHue(3);
}

// okay but what if it was all rings
void IdlePatternFour_fullsend()
{
  drawRainbowWheel(100);
}

void IdlePatternSix()
{
  CRGBPalette16 pal = PartyColors_p;
  drawWheelWithPalette(140, pal);
}

//each hex spirals out, and then spirals off out
void IdlePatternFive()
{
  for (int j = 0; j < NUMHEXES; j++)
  {
    leds[hexes[j].spiral[spiralIndex++]] = CHSV(255 / 7 * j + hueOffset, 200, 100);
  }

  spiralIndex %= 19;
  increaseHue(5);
}
#pragma endregion

#pragma region TEST_PATTERNS
// test trash, don't use
// or maybe re-visit, I dunno.
int test_counter = 0;
void TestPatternOne()
{
  leds[hexes[6].ring3[test_counter]] = CHSV(0, 0, BRIGHTNESS);
  test_counter = (test_counter + 1) % 12;
}

// it just sets ring 4.  It was used for testing rings.
void TestPatternTwo()
{
  setRingColor(leds, 4, CHSV(0, 255, BRIGHTNESS));
}

// spiral out from the center
int spiralCounter = 0;
int spiralCounter2 = NUM_LEDS / 3;
int spiralCounter3 = NUM_LEDS / 3 * 2;
void TestPatternThree()
{
  if (spiralCounter == 19)
    spiralCounter++; // it looks dumb when it hits that pixel
  if (spiralCounter2 == 19)
    spiralCounter2++; // it looks dumb when it hits that pixel
  if (spiralCounter3 == 19)
    spiralCounter3++; // it looks dumb when it hits that pixel
  leds[fullSpiral[spiralCounter++]] = CHSV(0, 255, BRIGHTNESS);
  leds[fullSpiral[spiralCounter2++]] = CHSV(85, 255, BRIGHTNESS);
  leds[fullSpiral[spiralCounter3++]] = CHSV(170, 255, BRIGHTNESS);
  spiralCounter %= (NUM_LEDS - 1);
  spiralCounter2 %= (NUM_LEDS - 1);
  spiralCounter3 %= (NUM_LEDS - 1);
}

// 3 rings, woo
int testFourCounters[] = {0, 0, 0, 0, 0, 0, 0, 0};
void TestPatternFour()
{
  // Serial.printf("Spiral %d led %d\n", spiralCounter, fullSpiral[spiralCounter]);
  leds[allRings[5][testFourCounters[0]++]] = CHSV(0, 244, BRIGHTNESS);
  leds[allRings[4][testFourCounters[1]++]] = CHSV(85, 244, BRIGHTNESS);
  leds[allRings[3][testFourCounters[2]++]] = CHSV(170, 244, BRIGHTNESS);

  testFourCounters[0] %= 30;
  testFourCounters[1] %= 24;
  testFourCounters[2] %= 18;
}

#pragma endregion

#pragma region FIRE_EXAMPLE
#define COOLING 55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120
CRGBPalette16 gPal = HeatColors_p;
bool gReverseDirection = true;

void Fire2012WithPalette()
{
  // Array of temperature readings at each simulation cell
  random16_add_entropy(random());
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for (int i = 0; i < NUM_LEDS; i++)
  {
    heat[i] = qsub8(heat[i], random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for (int k = NUM_LEDS - 1; k >= 2; k--)
  {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if (random8() < SPARKING)
  {
    int y = random8(7);
    heat[y] = qadd8(heat[y], random8(160, 255));
  }

  // Step 4.  Map from heat cells to LED colors
  for (int j = 0; j < NUM_LEDS; j++)
  {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    byte colorindex = scale8(heat[j], 240);
    CRGB color = ColorFromPalette(gPal, colorindex);
    int pixelnumber;
    if (gReverseDirection)
    {
      pixelnumber = (NUM_LEDS - 1) - j;
    }
    else
    {
      pixelnumber = j;
    }
    leds[fullSpiral[pixelnumber]] = color;
    // leds[pixelnumber] = color;
  }
}

void FireCool()
{

  // Second, this palette is like the heat colors, but blue/aqua instead of red/yellow
  gPal = CRGBPalette16(CRGB::Black, CRGB::Blue, CRGB::Aqua, CRGB::White);
  Fire2012WithPalette();
}
#pragma endregion

#pragma region PACIFICA_EXAMPLE

//////////////////////////////////////////////////////////////////////////
//
// The code for this animation is more complicated than other examples, and
// while it is "ready to run", and documented in general, it is probably not
// the best starting point for learning.  Nevertheless, it does illustrate some
// useful techniques.
//
//////////////////////////////////////////////////////////////////////////
//
// In this animation, there are four "layers" of waves of light.
//
// Each layer moves independently, and each is scaled separately.
//
// All four wave layers are added together on top of each other, and then
// another filter is applied that adds "whitecaps" of brightness where the
// waves line up with each other more.  Finally, another pass is taken
// over the led array to 'deepen' (dim) the blues and greens.
//
// The speed and scale and motion each layer varies slowly within independent
// hand-chosen ranges, which is why the code has a lot of low-speed 'beatsin8' functions
// with a lot of oddly specific numeric ranges.
//
// These three custom blue-green color palettes were inspired by the colors found in
// the waters off the southern coast of California, https://goo.gl/maps/QQgd97jjHesHZVxQ7
//
CRGBPalette16 pacifica_palette_1 =
    {0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117,
     0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50};
CRGBPalette16 pacifica_palette_2 =
    {0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117,
     0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F};
CRGBPalette16 pacifica_palette_3 =
    {0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33,
     0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF};

// Add one layer of waves into the led array
void pacifica_one_layer(CRGBPalette16 &p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    waveangle += 250;
    uint16_t s16 = sin16(waveangle) + 32768;
    uint16_t cs = scale16(s16, wavescale_half) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16(ci) + 32768;
    uint8_t sindex8 = scale16(sindex16, 240);
    CRGB c = ColorFromPalette(p, sindex8, bri, LINEARBLEND);
    // leds[i] += c;
    leds[fullSpiral[i]] += c;
  }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps()
{
  uint8_t basethreshold = beatsin8(9, 55, 65);
  uint8_t wave = beat8(7);

  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    uint8_t threshold = scale8(sin8(wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if (l > threshold)
    {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8(overage, overage);
      leds[fullSpiral[i]] += CRGB(overage, overage2, qadd8(overage2, overage2));
      // leds[i] += CRGB( overage, overage2, qadd8( overage2, overage2));
    }
  }
}

// Deepen the blues and greens
void pacifica_deepen_colors()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    // leds[i].blue = scale8( leds[i].blue,  145);
    // leds[i].green= scale8( leds[i].green, 200);
    // leds[i] |= CRGB( 2, 5, 7);
    leds[fullSpiral[i]].blue = scale8(leds[fullSpiral[i]].blue, 145);
    leds[fullSpiral[i]].green = scale8(leds[fullSpiral[i]].green, 200);
    leds[fullSpiral[i]] |= CRGB(2, 5, 7);
  }
}

void pacifica_loop()
{
  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  static uint32_t sLastms = 0;
  uint32_t ms = GET_MILLIS();
  uint32_t deltams = ms - sLastms;
  sLastms = ms;
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011, 10, 13));
  sCIStart2 -= (deltams21 * beatsin88(777, 8, 11));
  sCIStart3 -= (deltams1 * beatsin88(501, 5, 7));
  sCIStart4 -= (deltams2 * beatsin88(257, 4, 6));

  // Clear out the LED array to a dim background blue-green
  fill_solid(leds, NUM_LEDS, CRGB(2, 6, 10));

  // Render each of four layers, with different scales and speeds, that vary over time
  pacifica_one_layer(pacifica_palette_1, sCIStart1, beatsin16(3, 11 * 256, 14 * 256), beatsin8(10, 70, 130), 0 - beat16(301));
  pacifica_one_layer(pacifica_palette_2, sCIStart2, beatsin16(4, 6 * 256, 9 * 256), beatsin8(17, 40, 80), beat16(401));
  pacifica_one_layer(pacifica_palette_3, sCIStart3, 6 * 256, beatsin8(9, 10, 38), 0 - beat16(503));
  pacifica_one_layer(pacifica_palette_3, sCIStart4, 5 * 256, beatsin8(8, 10, 28), beat16(601));

  // Add brighter 'whitecaps' where the waves lines up more
  pacifica_add_whitecaps();

  // Deepen the blues and greens a bit
  pacifica_deepen_colors();
}
#pragma endregion

#endif