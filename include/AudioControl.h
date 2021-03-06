#ifndef AudioControl_h
#define AudioControl_h

#include <Audio.h>
#include "BeatDetector.h"
#define LOG_FFT false
bool logLevel[] = {false, false, false, false, false, false, false, false}; // if you want to get particular
const int myInput = AUDIO_INPUT_LINEIN;
#define BEATTIME 20

// GUItool: begin automatically generated code
AudioInputI2S i2s2; //xy=158,468
AudioMixer4 mixer1; //xy=419,510
AudioOutputI2S           i2s1;           //xy=557,730
AudioAnalyzeFFT256 fft256_1;   //xy=467,147
AudioAnalyzeFFT1024 fft1024_1; //xy=467,147

// INPUT
AudioConnection patchCord1(i2s2, 0, mixer1, 0);
AudioConnection patchCord2(i2s2, 1, mixer1, 1);

// OUTPUT
AudioConnection          patchCord3(mixer1, 0, i2s1, 0);
AudioConnection          patchCord4(mixer1, 0, i2s1, 1);

// FFT
AudioConnection patchCord5(mixer1, fft256_1);
AudioConnection patchCord6(mixer1, fft1024_1);

AudioControlSGTL5000 audioShield;

BeatDetector beatDetector(fft256_1);

void logLevels(float levels[8])
{
  if (!LOG_FFT)
  {
    return;
  }
  for (int i = 0; i < 8; i++)
  {
    if (logLevel[i])
    {
      int band_value = int(levels[i]);
      Serial.printf("%d:%d ", i, band_value);
    }
  }

}

float *LoadLevels(float levels[8])
{
  // read the 256 FFT frequencies into 16 levels
  // music is heard in octaves, but the FFT data
  // is linear, so for the higher octaves, read
  // many FFT bins together.
  levels[0] = fft256_1.read(0, 0) * 20;
  levels[1] = fft256_1.read(1, 2) * 20;
  levels[2] = fft256_1.read(3, 5) * 20;
  levels[3] = fft256_1.read(6, 13) * 20;
  levels[4] = fft256_1.read(14, 30) * 20;
  levels[5] = fft256_1.read(31, 61) * 20;
  levels[6] = fft256_1.read(62, 125) * 30;
  levels[7] = fft256_1.read(126, 254) * 30;
  logLevels(levels);
  return levels;
}

float *LoadLevels1024(float levels[8])
{
  // read the 1024 FFT frequencies into 16 levels
  // music is heard in octaves, but the FFT data
  // is linear, so for the higher octaves, read
  // many FFT bins together.
  levels[0] = fft1024_1.read(0, 2) * 20;
  levels[1] = fft1024_1.read(3, 4) * 20;
  levels[2] = fft1024_1.read(5, 10) * 20;
  levels[3] = fft1024_1.read(11, 22) * 20;
  levels[4] = fft1024_1.read(23, 49) * 20;
  levels[5] = fft1024_1.read(50, 107) * 20;
  levels[6] = fft1024_1.read(108, 234) * 20;
  levels[7] = fft1024_1.read(235, 511) * 20; // this one needs a pretty heavy boost
  logLevels(levels);
  return levels;
}

void init_audio()
{
  AudioMemory(48);
  audioShield.enable();
  audioShield.inputSelect(myInput);
  audioShield.volume(0.5);
  mixer1.gain(0, .5);
  mixer1.gain(1, .5);
  fft256_1.averageTogether(3);
  fft1024_1.averageTogether(3);
}

bool lowBeat = false;
bool virtualBeat = false;
bool bpmBeat = false;
elapsedMillis lowBeatLedTimer = 0;
elapsedMillis virtualLedTimer = 0;
elapsedMillis validBpmLedTimer = 0;
void beatDetectionLoop()
{
  beatDetector.BeatDetectorLoop();

  if (beatDetector.lowBeat) //Beat has been detected
  {
    lowBeatLedTimer = 0; //zero timer so led will be turned on
  }

  if (beatDetector.virtualBeat) //virtual beat generaget from detected Beats and generating a bpm
  {
    virtualLedTimer = 0; //zero timer so led will be turned on
  }

  if (beatDetector.validBPM) //valid bpm detected so virtual beat will be syncronised with this beat
  {
    validBpmLedTimer = 0; //zero timer so led will be turned on
  }

  lowBeat = lowBeatLedTimer < BEATTIME; //Beat has been detected above

  virtualBeat = virtualLedTimer < BEATTIME; //Beat has been detected above

  bpmBeat = validBpmLedTimer < BEATTIME; //validBPM

  // Serial.printf("low:%d ",lowBeat);
  // Serial.printf("virtual:%d ",virtualBeat);
  // Serial.printf("bpm:%d ",bpmBeat);
}

bool getLowBeat()
{
  // Serial.println(lowBeat);
  return lowBeat;
}

bool getVirtualBeat()
{
  // Serial.printf("virtual:%d ",virtualBeat);
  return virtualBeat;
}

bool getBpmBeat()
{
  return bpmBeat;
}

#endif