/*
Midi CC output
currently set to control shurti synth
remove the 'map' if the output of the midi needs to be 0-127

*/

    
    #include "MIDIUSB.h"
  
              uint8_t old1Knob = 0;
              uint8_t new1Knob = 0;
          
              uint8_t old2Knob = 0;
              uint8_t new2Knob = 0;
          
              uint8_t old3Knob = 0;
              uint8_t new3Knob = 0;
          
              uint8_t old4Knob = 0;
              uint8_t new4Knob = 0;
          
              uint8_t old5Knob = 0;
              uint8_t new5Knob = 0;
          
              uint8_t old6Knob = 0;
              uint8_t new6Knob = 0;
          
              uint8_t old7Knob = 0;
              uint8_t new7Knob = 0;
          
              uint8_t old8Knob = 0;
              uint8_t new8Knob = 0;
  
    void setup()
    {}
    
    void loop()
    {
      // 1knob :  cut off  //  CC = 14    //  Range = 0 - 127
    
            uint16_t oneknob = analogRead(A0);
            uint8_t new1Knob = oneknob >> 3;
    
      if ( new1Knob != old1Knob )
      {
            midiEventPacket_t midiCc = { 0x0B, 0xB0, 14, new1Knob };
            MidiUSB.sendMIDI(midiCc);
            MidiUSB.flush();
            oneknob = new1Knob;
      }
    
      // 2knob :   res  //  CC = 15    // Range =  0 - 63
    
    
            uint16_t twoknob = analogRead(A1);
            uint8_t new2Knob = twoknob >> 3;
    
      if ( new2Knob != old2Knob )
      {
            midiEventPacket_t midiCc = { 0x0B, 0xB0, 15, new2Knob / 2 };
            MidiUSB.sendMIDI(midiCc);
            MidiUSB.flush();
            twoknob = new2Knob;
      }
    
      //  3knob :   osc 1 shape  // CC = 20     // Range = 0 - 34
    
            uint16_t threeknob = analogRead(A2);
            uint8_t new3Knob = threeknob >> 3;
          //new3Knob = map(new3Knob, 0, 127, 0, 127);
    
      if ( new3Knob != old3Knob )
      {
            midiEventPacket_t midiCc = { 0x0B, 0xB0, 20, new3Knob };
            MidiUSB.sendMIDI(midiCc);
            MidiUSB.flush();
            threeknob = new3Knob;
      }
    
    
      // 4knob :   osc1 pwm     // CC =  21     // Range 0 - 127
    
            uint16_t fourknob = analogRead(A3);
            uint8_t new4Knob = fourknob >> 3;
      
      if ( new4Knob != old4Knob )
      {
            midiEventPacket_t midiCc = { 0x0B, 0xB0, 21, new4Knob };
            MidiUSB.sendMIDI(midiCc);
            MidiUSB.flush();
            fourknob = new4Knob;
      }
    
    
      //   5knob :   lfo 1 rate  // CC = 113    // Range = 0 - 143
    
    
            uint16_t fiveknob = analogRead(A4);
            uint8_t new5Knob = fiveknob >> 3;
          
    
      if ( new5Knob != old5Knob )
      {
            midiEventPacket_t midiCc = { 0x0B, 0xB0, 113, new5Knob };
            MidiUSB.sendMIDI(midiCc);
            MidiUSB.flush();
            fiveknob = new5Knob;
      }
    
    
      //   6knob :   osc2  // CC =  24     //  Range 0 - 34
    
    
            uint16_t sixknob = analogRead(A5);
            uint8_t new6Knob = sixknob >> 3;
          //new6Knob = map(new6Knob, 0, 127, 0, 34);
    
      if ( new6Knob != old6Knob )
      {
            midiEventPacket_t midiCc = { 0x0B, 0xB0, 24, new6Knob };
            MidiUSB.sendMIDI(midiCc);
            MidiUSB.flush();
            sixknob = new6Knob;
      }
    
    
      //   7knob :   osc2 para   // CC =  25     // Range = 0 - 127
    
    
            uint16_t sevenknob = analogRead(A6);
            uint8_t new7Knob = sevenknob >> 3;
    
      if ( new7Knob != old7Knob )
      {
            midiEventPacket_t midiCc = { 0x0B, 0xB0, 25, new7Knob };
            MidiUSB.sendMIDI(midiCc);
            MidiUSB.flush();
            sevenknob = new7Knob;
      }
    
    
      //   8knob :   env 1 R   CC = 107    //  Range = 0 - 127
    
    
            uint16_t eightknob = analogRead(A7);
            uint8_t new8Knob = eightknob >> 3;
    
      if ( new8Knob != old8Knob )
      {
            midiEventPacket_t midiCc = { 0x0B, 0xB0, 107, new8Knob };
            MidiUSB.sendMIDI(midiCc);
            MidiUSB.flush();
            eightknob = new8Knob;
      }
    
    }
