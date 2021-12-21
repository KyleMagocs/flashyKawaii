#ifndef Palette_h
#define Palette_h

#include <FastLED.h>

CRGBPalette16 currentPalette = Rainbow_gp;

#pragma region palettes
CRGB purple = CHSV(HUE_PURPLE, 255, 255);
CRGB green = CHSV(HUE_GREEN, 255, 255);
CRGB black = CRGB::Black;
CRGB white = CRGB::White;

CRGBPalette16 GreenAndPurple_p = CRGBPalette16(
    green, green, green, green,
    black, black, black, black,
    purple, purple, purple, purple,
    black, black, black, black);

CRGBPalette16 Matrix_p = CRGBPalette16(
    green, green, black, black,
    white, black, black, green,
    black, green, black, white,
    black, green, black, green);

CRGBPalette16 WhiteStripes_p = CRGBPalette16(
    white, white, white, white,
    black, black, black, black,
    white, white, white, white,
    black, black, black, black);

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

// Gradient palette "Dark2_06_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/cb/qual/tn/Dark2_06.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 48 bytes of program space.
DEFINE_GRADIENT_PALETTE( Dark2_06_gp ) {
    0,   1, 88, 37,
   42,   1, 88, 37,
   42, 167, 29,  1,
   84, 167, 29,  1,
   84,  33, 41,105,
  127,  33, 41,105,
  127, 197,  4, 54,
  170, 197,  4, 54,
  170,  23, 99,  1,
  212,  23, 99,  1,
  212, 194,105,  1,
  255, 194,105,  1};

// Gradient palette "German_flag_smooth_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ggr/tn/German_flag_smooth.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.
DEFINE_GRADIENT_PALETTE( German_flag_smooth_gp ) {
    0,   0,  0,  0,
   63,  42,  0,  0,
  127, 255,  0,  0,
  191, 255, 55,  0,
  255, 255,255,  0};

// Gradient palette "aurora_borealis_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pd/astro/tn/aurora_borealis.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 196 bytes of program space.
DEFINE_GRADIENT_PALETTE( aurora_borealis_gp ) {
    0,  16,  6, 15,
    5,  18,  7, 16,
   10,  20,  8, 17,
   15,  21,  8, 18,
   21,  22,  8, 19,
   26,  24,  9, 21,
   31,  27, 10, 22,
   37,  29, 10, 23,
   42,  32, 11, 24,
   47,  35, 11, 25,
   53,  38, 12, 27,
   58,  42, 13, 28,
   63,  46, 15, 30,
   69,  48, 16, 31,
   74,  50, 16, 31,
   79,  54, 16, 31,
   84,  58, 16, 31,
   90,  66, 18, 35,
   95,  74, 21, 40,
  100,  78, 21, 38,
  106,  82, 20, 37,
  111,  87, 22, 38,
  116,  92, 23, 40,
  122,  97, 23, 39,
  127, 101, 23, 38,
  132, 107, 25, 40,
  138, 112, 26, 42,
  143, 121, 29, 44,
  148, 132, 31, 46,
  154, 137, 36, 49,
  159, 142, 41, 52,
  164, 148, 48, 55,
  170, 153, 56, 60,
  175, 153, 68, 64,
  180, 155, 81, 69,
  185, 150,100, 68,
  191, 142,122, 67,
  196, 130,146, 63,
  201, 118,169, 59,
  207, 100,184, 52,
  212,  84,197, 48,
  217,  73,203, 42,
  223,  64,209, 36,
  228,  50,195, 29,
  233,  39,182, 23,
  239,  32,119, 23,
  244,  26, 71, 24,
  249,  24, 46, 25,
  255,  22, 28, 26};

// Gradient palette "stratosphere_sunset_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pd/astro/tn/stratosphere_sunset.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 236 bytes of program space.
DEFINE_GRADIENT_PALETTE( stratosphere_sunset_gp ) {
    0,   1,  2,  9,
    4,   1,  2,  9,
    8,   1,  2, 10,
   13,   1,  2, 11,
   17,   1,  2, 11,
   21,   1,  3, 13,
   26,   1,  3, 15,
   30,   1,  3, 16,
   35,   1,  4, 18,
   39,   1,  4, 19,
   43,   1,  4, 21,
   48,   1,  5, 23,
   52,   1,  5, 27,
   57,   1,  6, 32,
   61,   1,  7, 38,
   65,   1,  9, 49,
   70,   1, 11, 61,
   74,   1, 17, 80,
   79,   1, 23,103,
   83,   1, 31,135,
   87,   2, 42,172,
   92,   4, 48,190,
   96,   9, 54,210,
  101,  11, 55,214,
  105,  13, 58,221,
  109,  16, 60,219,
  114,  19, 62,216,
  118,  24, 66,210,
  123,  30, 69,201,
  127,  35, 70,180,
  131,  39, 70,162,
  136,  64, 84,144,
  140,  97, 97,128,
  145, 148,121,123,
  149, 210,146,119,
  153, 184,122, 92,
  158, 159,100, 69,
  162, 133, 68, 33,
  167, 110, 42, 11,
  171, 152, 53,  6,
  175, 203, 65,  3,
  180, 222, 99,  5,
  184, 242,142,  7,
  189, 222,122,  5,
  193, 203,105,  3,
  197, 208, 73,  1,
  202, 210, 48,  1,
  206, 222, 45,  1,
  211, 234, 42,  1,
  215, 150, 23,  1,
  219,  86, 10,  1,
  224,  35,  3,  1,
  228,   9,  1,  1,
  233,   4,  1,  1,
  237,   1,  1,  1,
  241,   1,  1,  1,
  246,   1,  1,  1,
  250,   1,  1,  1,
  255,   1,  1,  1};

// Gradient palette "ramp_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/grass/tn/ramp.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.
DEFINE_GRADIENT_PALETTE( ramp_gp ) {
    0,   0,  0,  0,
   84,   0,  0,255,
   84,   0,  0,  0,
  170,   0,255,  0,
  170,   0,  0,  0,
  255, 255,  0,  0};

// Gradient palette "GMT_seis_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/gmt/tn/GMT_seis.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 40 bytes of program space.
DEFINE_GRADIENT_PALETTE( GMT_seis_gp ) {
    0,  88,  0,  0,
   28, 255,  0,  0,
   56, 255, 22,  0,
   85, 255,104,  0,
  113, 255,255,  0,
  141, 255,255,  0,
  169,  17,255,  1,
  198,   0,223, 31,
  226,   0, 19,255,
  255,   0,  0,147};

// Gradient palette "winter_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/h5/tn/winter.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.
DEFINE_GRADIENT_PALETTE( winter_gp ) {
    0,   0,  0,255,
  255,   0,255, 44};

// Gradient palette "cool_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/h5/tn/cool.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.
DEFINE_GRADIENT_PALETTE( cool_gp ) {
    0,   0,255,255,
  255, 255,  0,255};

// Gradient palette "yarg_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/h5/tn/yarg.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.
DEFINE_GRADIENT_PALETTE( yarg_gp ) {
    0,   0,  0,  0,
  255, 255,255,255};


CRGBPalette16 allPalettes[] = {
    stratosphere_sunset_gp,
    aurora_borealis_gp,
    Dark2_06_gp,
    bhw2_50_gp,
    GMT_seis_gp,
    ramp_gp,
    GreenAndPurple_p,
    cool_gp,
    yarg_gp,
    // WhiteStripes_p // ack it just looks like ass because I'm not injecting better power
    winter_gp
};
int numPalettes = sizeof(allPalettes) / sizeof(CRGBPalette16);
#pragma endregion

CRGBPalette16 getCurrentPalette(){
    return currentPalette;
}

void randomizePalette(){
    currentPalette = allPalettes[random(numPalettes)];
}


#endif