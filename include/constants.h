#ifndef Constants_h
#define Constants_h

#define PATTERN_TIMER 30
#define IDLE_TIMEOUT 45
#define FPS 15
#define MAGIC_NUMBER 2400  // used to calculate decay factor
#define DECAY MAGIC_NUMBER/FPS

#define BASS_THRESHOLD 13
#define BASS_TIMEOUT 4

#define BRIGHTNESS 200
#define IDLEBRIGHTNESS 150
#define NUM_LEDS_FULL 150 // only actually 133, sorta 134, but FastLED is pulling some shenanigans, so, we need to define the full 3 strips.
#define NUM_LEDS 134

#endif