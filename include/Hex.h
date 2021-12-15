
#ifndef Hex_h
#define Hex_h

// hex definitions
#define INNERLEN 1
#define MIDDLELEN 6
#define OUTERLEN 12
#define NUMHEXES 7

#define NUM_LEDS    134

class Hex
{
  public:
    Hex(int index);
    Hex();
    int _index;
    int center[INNERLEN];
    int middle[MIDDLELEN];
    int outer[OUTERLEN];
    int *ring1;
    int *ring2;
    int *ring3;
    int *ring4;  // here and below the numbers drop because rings are shared between multiple hexes
    int *ring5;
    int *ring6;
    int *ring7;
    int *ring8;
    int spiral[19];
};
Hex::Hex() { }

Hex::Hex(int index)
{
  _index = index;
  int mod = index * 19;

  // to ease programming, a pixel was skipped in the wiring, so we gotta do this.
  // or I guess I could have cut the wire and soldered a tiny extension in?
  // but fixing it in code is way easier
  // I'm pretty bad at soldering
  // and then if the strip died I couldn't just drop a new one in
  // so here we are
  // wow this is a long comment
  // hello future socks and/or github viewers
  if (index >= 6) {
    mod += 1;
  }


  center[0] = 9 + mod;

  middle[0] = mod + 3;
  middle[1] = mod + 15;
  middle[2] = mod + 10;
  middle[3] = mod + 14;
  middle[4] = mod + 4;
  middle[5] = mod + 8;

  outer[0] = 11 + mod;
  outer[1] = 12 + mod;
  outer[2] = 13 + mod;
  outer[3] = 2 + mod;
  outer[4] = 16 + mod;
  outer[5] = 1 + mod;
  outer[6] = 17 + mod;
  outer[7] = 0 + mod;
  outer[8] = 18 + mod;
  outer[9] = 5 + mod;
  outer[10] = 6 + mod;
  outer[11] = 7 + mod;

  spiral[0] = 9 + mod;


  spiral[1] = 4 + mod;
  spiral[2] = 8 + mod;
  spiral[3] = 15 + mod;
  spiral[4] = 14 + mod;
  spiral[5] = 10 + mod;
  spiral[6] = 3 + mod;

  spiral[7] = 0 + mod;
  spiral[8] = 5 + mod;
  spiral[9] = 6 + mod;
  spiral[10] = 7 + mod;
  spiral[11] = 18 + mod;
  spiral[12] = 17 + mod;
  spiral[13] = 16 + mod;
  spiral[14] = 13 + mod;
  spiral[15] = 12 + mod;
  spiral[16] = 11 + mod;
  spiral[17] = 2 + mod;
  spiral[18] = 1 + mod;


  if (index == 6) { // center ring is different, obv
    ring1 = new int[1] { 9 + mod }; // I'm tired of trying to deal with pointers
    ring2 = new int[6] { mod + 3, mod + 15, mod + 10, mod + 14, mod + 4, mod + 8 };
    ring3 = new int[12] { 11 + mod, 12 + mod, 13 + mod, 16 + mod, 17 + mod, 18 + mod, 7 + mod, 6 + mod, 5 + mod,  0 + mod,  1 + mod, 2 + mod };
    ring4 = NULL;
    ring5 = NULL;
    ring6 = NULL;
    ring7 = NULL;
    ring8 = NULL;
  }
  else {
    ring1 = NULL;
    ring2 = NULL;
    ring3 = NULL;
    ring8 = new int[3] { 11 + mod, 12 + mod, 13 + mod };
    ring7 = new int[4] { 18 + mod, 8 + mod, 4 + mod, 0 + mod };
    ring6 = new int[5] { 17 + mod, 15 + mod, 9 + mod, 3 + mod, 1 + mod };
    ring5 = new int[4] { 16 + mod, 14 + mod, 10 + mod, 2 + mod };
    ring4 = new int[3] { 7 + mod, 6 + mod, 5 + mod };
  }

  //  Serial.print("len of ring1: ");
  //  Serial.println(sizeof(ring1) / sizeof(int));
  //  Serial.print("index of ring1[0]: ");
  //  Serial.println(ring1 == NULL);
  //  Serial.println("");
}

// all 8 rings of the full install
Hex hexes[7];
int ringLens[] = { 1, 6, 12, 3, 4, 5, 4, 3 };
int outline[42];
int outlineCounter = 0;
void init_hexes() {
  // bespoke ordering, why not.
  hexes[6] = Hex(6);
  hexes[5] = Hex(0);
  hexes[4] = Hex(3);
  hexes[3] = Hex(1);
  hexes[2] = Hex(4);
  hexes[1] = Hex(2);
  hexes[0] = Hex(5);
  for (int i = 0; i < 6; i++)
  {
    outline[outlineCounter++] = 1  + (i * 19);
    outline[outlineCounter++] = 2  + (i * 19);
    outline[outlineCounter++] = 11 + (i * 19);
    outline[outlineCounter++] = 12 + (i * 19);
    outline[outlineCounter++] = 13 + (i * 19);
    outline[outlineCounter++] = 16 + (i * 19);
    outline[outlineCounter++] = 17 + (i * 19);
  }
}

void setRingColor(CRGBArray<NUM_LEDS> &leds, int ring, CHSV color) {  // todo:  this would be a lot easier if you weren't fucked up and you could handle string concat, my dude
  switch (ring) {
    case 0:
      for (int i = 0; i < NUMHEXES; i++)
      {
        if (hexes[i].ring1 != NULL) {
          for (int j = 0; j < ringLens[ring]; j++) {
            leds[hexes[i].ring1[j]] = color;
          }
        }
      }
      break;
    case 1:
      for (int i = 0; i < NUMHEXES; i++)
      {
        if (hexes[i].ring2 != NULL) {
          for (int j = 0; j < ringLens[ring]; j++) {
            leds[hexes[i].ring2[j]] = color;
          }
        }
      }
      break;
    case 2:
      for (int i = 0; i < NUMHEXES; i++)
      {
        if (hexes[i].ring3 != NULL) {
          for (int j = 0; j < ringLens[ring]; j++) {
            leds[hexes[i].ring3[j]] = color;
          }
        }
      }
      break;
    case 3:
      for (int i = 0; i < NUMHEXES; i++)
      {
        if (hexes[i].ring4 != NULL) {
          for (int j = 0; j < ringLens[ring]; j++) {
            leds[hexes[i].ring4[j]] = color;
          }
        }
      }
      break;
    case 4:
      for (int i = 0; i < NUMHEXES; i++)
      {
        if (hexes[i].ring5 != NULL) {
          for (int j = 0; j < ringLens[ring]; j++) {
            leds[hexes[i].ring5[j]] = color;
          }
        }
      }
      break;
    case 5:
      for (int i = 0; i < NUMHEXES; i++)
      {
        if (hexes[i].ring6 != NULL) {
          for (int j = 0; j < ringLens[ring]; j++) {
            leds[hexes[i].ring6[j]] = color;
          }
        }
      }
      break;
    case 6:
      for (int i = 0; i < NUMHEXES; i++)
      {
        if (hexes[i].ring7 != NULL) {
          for (int j = 0; j < ringLens[ring]; j++) {
            leds[hexes[i].ring7[j]] = color;
          }
        }
      }
      break;
    case 7:
      for (int i = 0; i < NUMHEXES; i++)
      {
        if (hexes[i].ring8 != NULL) {
          for (int j = 0; j < ringLens[ring]; j++) {
            leds[hexes[i].ring8[j]] = color;
          }
        }
      }
      break;
  }
}
#endif
