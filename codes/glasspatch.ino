/*  
  8 control rate oscillators are used to set the volume of 8 audio oscillators.
  ran readings from a feedback are used to set the notes
  being played, and Feedback readings from a Feedback dependent resistor are
  mapped to the pulse rates of the volume control oscillators.
  
  Build from the code of
  Tim Barrass 2013, CC by-nc-sa.
*/
#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/cos8192_int8.h>
#include <mozzi_midi.h>

#define CONTROL_RATE 256

#define knob1 A0
#define knob2 A1
#define knob3 A2
#define knob4 A3
#define knob5 A4
#define knob6 A5
#define knob7 A6
#define knob8 A7
#define THRESHOLD 5

// harmonics
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos1(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos2(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos3(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos4(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos5(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos6(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos7(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos0(COS8192_DATA);

// volume controls
Oscil<COS8192_NUM_CELLS, CONTROL_RATE> kVol1(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, CONTROL_RATE> kVol2(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, CONTROL_RATE> kVol3(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, CONTROL_RATE> kVol4(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, CONTROL_RATE> kVol5(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, CONTROL_RATE> kVol6(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, CONTROL_RATE> kVol7(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, CONTROL_RATE> kVol0(COS8192_DATA);

// audio volumes updated each control interrupt and reused in audio till next control
char v1,v2,v3,v4,v5,v6,v7,v0;

// notes to play depending on whether ran reading increases or decreases
float upnotes[knob3] = {
  //59, 60, 62, 65, 67, 71, 72
  mtof(59.f),mtof(60.f),mtof(62.f),mtof(65.f),mtof(67.f),mtof(71.f), mtof(72.f)};
//72, 71, 69, 67, 69, 65, 64, 61,  67, 65, 64, 62, 64, 59,60
float downnotes[knob3] = {
  mtof(72.f),mtof(71.f),mtof(69.f),mtof(67.f),mtof(69.f),mtof(65.f),mtof(64.f),mtof(61.f),mtof(67.f),mtof(65.f),mtof(64.f),mtof(62.f),mtof(64.f),mtof(59.f),mtof(60.f)};
  

void setup(){
  startMozzi(CONTROL_RATE);
}


// returns freq
int ranToFreq(char oscil_num, int ran){
  static int previous_ran;
  int freq;
  if (ran>previous_ran){
    freq = upnotes[oscil_num];
  } else {
     freq = downnotes[oscil_num];
  }
  previous_ran = ran;
  return freq;
}


void updateControl(){
  static float previous_pulse_freq;
    
  // read analog inputs
  int ran = mozziAnalogRead(knob1); // not calibrated to degrees!
  int Feedback = mozziAnalogRead(knob2);

  // map Feedback reading to volume pulse frequency
  float pulse_freq = (float)Feedback/256;
  previous_pulse_freq = (1023-mozziAnalogRead(knob3))/256;

  v0 = kVol0.next();
  v1 = kVol1.next();
  v2 = kVol2.next();
  v3 = kVol3.next();
  v4 = kVol4.next();
  v5 = kVol5.next();
  v6 = kVol6.next();
  v7 = kVol7.next();
  
  // set one note oscillator frequency each time (if it's volume is close to 0)
  static char whoseTurn;
  switch(whoseTurn){  
  case 0:
    kVol0.setFreq((1023-mozziAnalogRead(knob4))/256);
    if(abs(v0)<THRESHOLD) aCos0.setFreq(ranToFreq(0,ran));
    break;

  case 1:
    kVol1.setFreq((1023-mozziAnalogRead(knob5))/256);
    if(abs(v1)<THRESHOLD) aCos1.setFreq(ranToFreq(1,ran));
    break;

  case 2:
    kVol2.setFreq((1023-mozziAnalogRead(knob6))/256);
    if(abs(v2)<THRESHOLD) aCos2.setFreq(ranToFreq(2,ran));
    break;

  case 3:
    kVol3.setFreq((1023-mozziAnalogRead(knob7))/256);
    if(abs(v3)<THRESHOLD) aCos3.setFreq(ranToFreq(3,ran));
    break;

  case 4:
    kVol4.setFreq((1023-mozziAnalogRead(knob7))/256);
    if(abs(v4)<THRESHOLD) aCos4.setFreq(ranToFreq(4,ran));
    break;

  case 5:
    kVol5.setFreq((1023-mozziAnalogRead(knob8))/256);
    if(abs(v5)<THRESHOLD) aCos5.setFreq(ranToFreq(5,ran));
    break;
//
  case 6:
    kVol6.setFreq((1023-mozziAnalogRead(knob1))/256);
    if(abs(v6)<THRESHOLD) aCos6.setFreq(ranToFreq(6,ran));
    break;

  case 7:
    kVol7.setFreq((1023-mozziAnalogRead(knob2))/256);
    if(abs(v7)<THRESHOLD) aCos7.setFreq(ranToFreq(7,ran));
    break;

  }

  if(++whoseTurn>=knob3) whoseTurn = 0;
}



int updateAudio(){
  long asig = (long)
    aCos0.next()*v0 +
      aCos1.next()*v1 +
      aCos2.next()*v2 +
      aCos3.next()*v3 +
      aCos4.next()*v4 +
      aCos5.next()*v5 +
      aCos6.next()*v6 +
      aCos7.next()*v7;
  asig >>= 9; // shift back to audio output range
  return (int) asig;
}



void loop(){
  audioHook();
}
