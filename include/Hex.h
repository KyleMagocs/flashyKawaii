
#ifndef Hex_h
#define Hex_h

// hex definitions
#define INNERLEN 1
#define MIDDLELEN 6
#define OUTERLEN 12
#define NUMHEXES 7

#define NUM_LEDS 134

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
  int *ring4; // here and below the numbers drop because rings are shared between multiple hexes
  int *ring5;
  int *ring6;
  int *ring7;
  int *ring8;
  int *rings[8];
  int spiral[19];
};
Hex::Hex() {}

Hex::Hex(int index)
{
  _index = index;
  int mod = index * 19;
  Serial.printf("initializing hex %d\n", index);
  // to ease programming, a pixel was skipped in the wiring, so we gotta do this.
  // or I guess I could have cut the wire and soldered a tiny extension in?
  // but fixing it in code is way easier
  // I'm pretty bad at soldering
  // and then if the strip died I couldn't just drop a new one in
  // so here we are
  // wow this is a long comment
  // hello future socks and/or github viewers
  if (index >= 6)
  {
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

  if (index == 6)
  {                              // center hex is different, obv
    ring1 = new int[1]{9 + mod}; // I'm tired of trying to deal with pointers
    ring2 = new int[6]{ mod + 10, mod + 14, mod + 15, mod + 8, mod + 4, mod + 3};
    ring3 = new int[12]{ 2 + mod, 11 + mod, 12 + mod, 13 + mod, 16 + mod, 17 + mod, 18 + mod, 7 + mod, 6 + mod, 5 + mod, 0 + mod, 1 + mod};
    ring4 = NULL;
    ring5 = NULL;
    ring6 = NULL;
    ring7 = NULL;
    ring8 = NULL;
  }
  else
  {
    ring1 = NULL;
    ring2 = NULL;
    ring3 = NULL;
    ring4 = new int[3]{5 + mod, 6 + mod, 7 + mod};
    ring5 = new int[4]{0 + mod, 4 + mod, 8 + mod, 18 + mod};
    ring6 = new int[5]{1 + mod, 3 + mod, 9 + mod, 15 + mod, 17 + mod};
    ring7 = new int[4]{2 + mod, 10 + mod, 14 + mod, 16 + mod};
    ring8 = new int[3]{11 + mod, 12 + mod, 13 + mod};
  }
  rings[0] = ring1;
  rings[1] = ring2;
  rings[2] = ring3;
  rings[3] = ring4;
  rings[4] = ring5;
  rings[5] = ring6;
  rings[6] = ring7;
  rings[7] = ring8;
  // ring4, ring5, ring6, ring7, ring8};
  //  Serial.print("len of ring1: ");
  //  Serial.println(sizeof(ring1) / sizeof(int));
  //  Serial.print("index of ring1[0]: ");
  //  Serial.println(ring1 == NULL);
  //  Serial.println("");
}

Hex hexes[7];
Hex orderedHexes[7];
int counter = 0;
int counter2 = 0;
int counter3 = 0;
int counter4 = 0;
int counter5 = 0;
int counter6 = 0;


int outline[42];
int outline2[36];
int outline3[30];
int outline4[6];
void initOutline()
{
  counter = 0;
  counter2 = 0;
  counter3 = 0;
  for (int i = 0; i < 6; i++)
  { // each hex is wired radially symetrically so we can just... do this.
    outline[counter++] = 1 + (i * 19);
    outline[counter++] = 2 + (i * 19);
    outline[counter++] = 11 + (i * 19);
    outline[counter++] = 12 + (i * 19);
    outline[counter++] = 13 + (i * 19);
    outline[counter++] = 16 + (i * 19);
    outline[counter++] = 17 + (i * 19);

    outline2[counter2++] = 0 + (i*19);
    outline2[counter2++] = 3 + (i*19);
    outline2[counter2++] = 10 + (i*19);
    outline2[counter2++] = 14 + (i*19);
    outline2[counter2++] = 15 + (i*19);
    outline2[counter2++] = 18 + (i*19);

    outline3[counter3++] = 5 + (i*19);
    outline3[counter3++] = 4 + (i*19);
    outline3[counter3++] = 9 + (i*19);
    outline3[counter3++] = 8 + (i*19);
    outline3[counter3++] = 7 + (i*19);

    outline4[counter4++] = 6 + (i*19);
  }
}

int fullSpiral[NUM_LEDS];
// all 8 rings of the full install, but per-hex
int ringLens[] = {1, 6, 12, 3, 4, 5, 4, 3};

// all 8 rings, total
int allRingLens[] = {1, 6, 12, 18, 24, 30, 24, 18};
int allRings[8][30];

void initSpiralandRings()
{
  counter = 0;
  counter2 = 0;
  for (int ring = 0; ring < 8; ring++)
  {
    counter2 = 0;
    // Serial.printf("Looking at ring %d\n", ring);
    for (int hex = 0; hex < NUMHEXES; hex++)
    {
      // Serial.printf("Looking at hex %d \n", hex);
      if (orderedHexes[hex].rings[ring] != NULL)
      {
        // Serial.printf("Found ring! \n");
        for (int j = 0; j < ringLens[ring]; j++)
        {
          // Serial.printf("Spiral %d > %d \n", counter, orderedHexes[hex].rings[ring][j]);
          fullSpiral[counter++] = orderedHexes[hex].rings[ring][j];

          // Serial.printf("Ring %d %d > %d\n", ring, counter2, orderedHexes[hex].rings[ring][j]);
          allRings[ring][counter2++] = orderedHexes[hex].rings[ring][j];
        }
      }
    }
  }
}

void init_hexes()
{
  Serial.println("Initializing hexes now");
  // bespoke ordering, why not.
  hexes[6] = Hex(6);
  hexes[5] = Hex(0);
  hexes[4] = Hex(3);
  hexes[3] = Hex(1);
  hexes[2] = Hex(4);
  hexes[1] = Hex(2);
  hexes[0] = Hex(5);
  for(int i = 0; i < NUMHEXES; i++){
    orderedHexes[i] = Hex(i);
  }

  Serial.println("hexes done");
  initOutline();
  Serial.println("outline done");
  initSpiralandRings();
  Serial.println("spirals and rings done");
}

void setRingColor(CRGBArray<NUM_LEDS> &leds, int ring, CHSV color)
{ 
  // Serial.printf("Gonna set ring %d\n", ring);
  for (int i = 0; i < allRingLens[ring]; i++)
  {
    leds[allRings[ring][i]] = color;
  }
}
#endif
