/* 
    8knobs Drones
    Analog Inputs: A0-A5, A6 - A11 (on digital pins 4, 6, 8, 9, 10, and 12). 
*/

//#include <ADC.h>  // Teensy 3.1 uncomment this line and install http://github.com/pedvide/ADC
#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin8192_int8.h>

#define CONTROL_RATE 128

// harmonics
Oscil<SIN8192_NUM_CELLS, AUDIO_RATE> aSin1(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, AUDIO_RATE> aSin2(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, AUDIO_RATE> aSin3(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, AUDIO_RATE> aSin4(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, AUDIO_RATE> aSin5(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, AUDIO_RATE> aSin6(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, AUDIO_RATE> aSin7(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, AUDIO_RATE> aSin8(SIN8192_DATA);

// volume controls
Oscil<SIN8192_NUM_CELLS, CONTROL_RATE> kVol1(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, CONTROL_RATE> kVol2(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, CONTROL_RATE> kVol3(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, CONTROL_RATE> kVol4(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, CONTROL_RATE> kVol5(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, CONTROL_RATE> kVol6(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, CONTROL_RATE> kVol7(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, CONTROL_RATE> kVol8(SIN8192_DATA);

// audio volumes updated each control interrupt and reused in audio till next control
char v1,v2,v3,v4,v5,v6,v7,v8;

void setup(){

  // set harmonic frequencies
  aSin1.setFreq(100);
  aSin2.setFreq(100);
  aSin3.setFreq(100);
  aSin4.setFreq(100);
  aSin5.setFreq(100);
  aSin6.setFreq(100);
  aSin7.setFreq(100);
  aSin8.setFreq(100);

// set volume change frequencies
  kVol1.setFreq(4.43f); // more of a pulse
  kVol2.setFreq(0.0245f);
  kVol3.setFreq(0.019f);
  kVol4.setFreq(0.07f);
  kVol5.setFreq(0.047f);
  kVol6.setFreq(0.031f);
  kVol7.setFreq(0.0717f);
  kVol8.setFreq(0.041f);

  v1=v2=v3=v4=v5=v6=v7=v8=127;
  
  startMozzi(CONTROL_RATE);
}

void loop(){
  audioHook();
}


void updateControl(){
  aSin1.setFreq((mozziAnalogRead(A0))*8);
  aSin2.setFreq((mozziAnalogRead(A1))*8);
  aSin3.setFreq((mozziAnalogRead(A2))*8);
  aSin4.setFreq((mozziAnalogRead(A3))*8);
  aSin5.setFreq((mozziAnalogRead(A4))*8);
  aSin6.setFreq((mozziAnalogRead(A5))*8);
  aSin7.setFreq((mozziAnalogRead(A6))*8);
  aSin8.setFreq((mozziAnalogRead(A7))*8);
   v1 = kVol1.next(); // going at a higher freq, this creates zipper noise, so reduce the gain
   v2 = kVol2.next();
   v3 = kVol3.next();
   v4 = kVol4.next();
   v5 = kVol5.next();
   v6 = kVol6.next();
   v7 = kVol7.next();
   v8 = kVol8.next();
}

int updateAudio(){
  long asig = (long)
    aSin1.next()*v1 +
    aSin2.next()*v2 +
    aSin3.next()*v3 +
    aSin4.next()*v4 +
    aSin5.next()*v5 +
    aSin6.next()*v6 +
    aSin7.next()*v7 +
    aSin8.next()*v8;
  asig >>= 9;
  return (int) asig;
}
