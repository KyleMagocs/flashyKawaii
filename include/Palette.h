#ifndef Palette_h
#define Palette_h

#include <FastLED.h>

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

// CRGBPalette16 bhw2_50_gp = CRGBPalette16(
//     0,   8,  2, 23,
//    84,  47,  7,102,
//   138,  16, 46,147,
//   173,   2,127,203,
//   255,   1,  7, 11);

#pragma endregion




#endif