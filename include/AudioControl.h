#include <Audio.h>
#define LOG_FFT false
const int myInput = AUDIO_INPUT_LINEIN;

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=158,468
AudioMixer4              mixer1;         //xy=419,510
//AudioOutputI2S           i2s1;           //xy=557,730
AudioAnalyzeFFT256      fft256_1;        //xy=467,147
//AudioConnection          patchCord1(i2s2, 0, i2s1, 0);
AudioConnection          patchCord2(i2s2, 0, mixer1, 1);
//AudioConnection          patchCord3(i2s2, 1, i2s1, 1);
AudioConnection          patchCord4(i2s2, 1, mixer1, 2);
AudioConnection          patchCord5(mixer1, fft256_1);

AudioControlSGTL5000 audioShield;

void logLevels(float levels[8]) {
  if ( !LOG_FFT ) return;
  for (int i = 0; i < 8; i++)
  {
    int band_value = int(levels[i]);
    Serial.print(String(band_value) + " ");
  }
  Serial.println("");
}

// float * LoadLevels(float levels[8]) {
//   // read the 256 FFT frequencies into 16 levels
//   // music is heard in octaves, but the FFT data
//   // is linear, so for the higher octaves, read
//   // many FFT bins together.
//   levels[0] = fft256_1.read(0) * 20;
//   levels[1] = fft256_1.read(1) * 20;
//   levels[2] = fft256_1.read(2) * 20;
//   levels[3] = fft256_1.read(3, 5) * 20;
//   levels[4] = fft256_1.read(6, 30) * 25;
//   levels[5] = fft256_1.read(31, 60) * 20;
//   levels[6] = fft256_1.read(61, 70) * 40;
//   levels[7] = fft256_1.read(71, 255) * 20;
//   logLevels(levels);
//   return levels;
// }

float * LoadLevels(float levels[8]) {
  // read the 256 FFT frequencies into 16 levels
  // music is heard in octaves, but the FFT data
  // is linear, so for the higher octaves, read
  // many FFT bins together.
  levels[0] = fft256_1.read(0,0) * 20;
  levels[1] = fft256_1.read(1,2) * 20;
  levels[2] = fft256_1.read(3,5) * 20;
  levels[3] = fft256_1.read(6, 13) * 20;
  levels[4] = fft256_1.read(14, 30) * 20;
  levels[5] = fft256_1.read(31, 61) * 20;
  levels[6] = fft256_1.read(62, 125) * 20;
  levels[7] = fft256_1.read(126, 254) * 20;
  logLevels(levels);
  return levels;
}




void init_audio() {
  AudioMemory(48);
  audioShield.enable();
  audioShield.inputSelect(myInput);
  audioShield.volume(0.5);
  mixer1.gain(0, 1);
  mixer1.gain(1, 1);
}
