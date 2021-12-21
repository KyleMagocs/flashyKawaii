#ifndef IdlePatterns_h
#define IdlePatterns_h

#include <SerialFlash.h>
#include <WS2812Serial.h>
#include "Hex.h"
#include "Palette.h"
#include "Patterns.h"
#include "AudioControl.h"

#define IDLEBRIGHTNESS 150

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
  spiralIndex %= 19;
  for (int j = 0; j < NUMHEXES; j++)
  {
    leds[hexes[j].spiral[spiralIndex++]] = CHSV(255 / 7 * j + hueOffset, 200, 100);
  }

  increaseHue(5);
}
#pragma endregion

#pragma region TEST_PATTERNS
// test trash, don't use
// or maybe re-visit, I dunno.
int test_counter = 0;
void TestPatternOne()
{
  leds[hexes[6].ring3[test_counter]] = CHSV(0, 0, IDLEBRIGHTNESS);
  test_counter = (test_counter + 1) % 12;
}

// it just sets ring 4.  It was used for testing rings.
void TestPatternTwo()
{
  setRingColor(leds, 4, CHSV(0, 255, IDLEBRIGHTNESS));
}

// spiral out from the center
int testThreeCounters[] = {0, 44, 88};
void TestPatternThree()
{
  decayFactor = 15;

  for(int i = 0; i < 3; i++){
    if (testThreeCounters[i] == 19) testThreeCounters[i]++;
  }

  leds[fullSpiral[testThreeCounters[0]++]] = CHSV(0, 255, IDLEBRIGHTNESS);
  leds[fullSpiral[testThreeCounters[1]++]] = CHSV(85, 255, IDLEBRIGHTNESS);
  leds[fullSpiral[testThreeCounters[2]++]] = CHSV(170, 255, IDLEBRIGHTNESS);
  testThreeCounters[0] %= (NUM_LEDS - 1);
  testThreeCounters[1] %= (NUM_LEDS - 1);
  testThreeCounters[2] %= (NUM_LEDS - 1);
}

int testThreeMatrixCounters[] = {0, 33, 66, 99};
void TestPatternThree_Matrix()
{
  decayFactor = 50;
  for(int i = 0; i < 4; i++){
    if (testThreeMatrixCounters[i] == 19) testThreeMatrixCounters[i]++;
    leds[fullSpiral[testThreeMatrixCounters[i]++]] = CHSV(85, 255, IDLEBRIGHTNESS);
    if (testThreeMatrixCounters[i] == 19) testThreeMatrixCounters[i]++; // lol I'm sure there's a better way to do this other than checking it twice but whatever
    testThreeMatrixCounters[i] %= (NUM_LEDS-1);
    leds[fullSpiral[testThreeMatrixCounters[i]]] = CHSV(85, 100, IDLEBRIGHTNESS-40);
  }
  delay(120); // slow it down.  slow it waaaaaaaaay down.
}

// 3 rings, woo
int testFourCounters[] = {0, 0, 0, 0, 0, 0, 0, 0};
void TestPatternFour()
{
  // Serial.printf("Spiral %d led %d\n", spiralCounter, fullSpiral[spiralCounter]);
  leds[allRings[5][testFourCounters[0]++]] = CHSV(0, 0, IDLEBRIGHTNESS);
  leds[allRings[4][testFourCounters[1]++]] = CHSV(85, 0, IDLEBRIGHTNESS);
  leds[allRings[3][testFourCounters[2]++]] = CHSV(170, 0, IDLEBRIGHTNESS);

  testFourCounters[0] %= 30;
  testFourCounters[1] %= 24;
  testFourCounters[2] %= 18;
}

// I did this on accident once so let's roll with it, eh?
void TestPatternFive()
{
  decayFactor = 15;

  for(int i = 0; i < 3; i++){
    if (testThreeCounters[i] == 114) testThreeCounters[i] = 115; // we skipped one in the wiring, so skip it here, too.
  }

  leds[testThreeCounters[0]++] = CHSV(0, 255, IDLEBRIGHTNESS);
  leds[testThreeCounters[1]++] = CHSV(85, 255, IDLEBRIGHTNESS);
  leds[testThreeCounters[2]++] = CHSV(170, 255, IDLEBRIGHTNESS);
  testThreeCounters[0] %= (NUM_LEDS);
  testThreeCounters[1] %= (NUM_LEDS);
  testThreeCounters[2] %= (NUM_LEDS);
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

void FireNormal(){
  gPal = HeatColors_p;
  Fire2012WithPalette();
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
// another filter is applied that adds "whitecaps" of IDLEBRIGHTNESS where the
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