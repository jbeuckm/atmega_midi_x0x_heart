#include <MIDI.h>
#include "AH_MCP4922/AH_MCP4922.h"
#include "DS1267/DS1267.h"

#define GATE_PIN 2
#define GATE_LED A5

#define ENV_MOD_CTRL 1
#define ENV_MOD_PIN 3

#define CUTOFF_CTRL 74
#define CUTOFF_PIN 6

#define SAW_CTRL 70
#define SAW_PIN 5
#define SQR_CTRL 71
#define SQR_PIN 9

#define SLIDE_CTRL 65
#define SLIDE_TIME_CTRL 5
#define SLIDE_IN_PIN 13
#define SLIDE_OUT_PIN 12

#define DECAY_CTRL 72
#define DECAY_PIN 11

#define ACCENT_CTRL 11
#define ACCENT_PIN 10

#define RES_CTRL 2
#define RES_POT_PIN A4

#define ALL_NOTES_OFF 123
#define CTRL_RESET 121

AH_MCP4922 PitchDac(A1,A2,A3,LOW,LOW);

// Velocity gets assigned to the analog cutoff voltage because 
// the PWM pins have some latency. Whatever velocity controls cannot 
// afford latency since the note is beginning simultaneously.
AH_MCP4922 CutoffDac(A1,A2,A3,HIGH,LOW);

// dual digipot to control x0x resonance programatically
DS1267 ResonancePot(8,7,4);


int liveNoteCount = 0;
int pitchbendOffset = 0;
int baseNoteFrequency;

byte deviceID;

MIDI_CREATE_DEFAULT_INSTANCE();


void handleNoteOn(byte channel, byte pitch, byte velocity)
{  
  liveNoteCount++;
  
  baseNoteFrequency = (pitch - 12) * 42;
  PitchDac.setValue(baseNoteFrequency + pitchbendOffset);

  CutoffDac.setValue(velocity * 32);

  digitalWrite(GATE_PIN, HIGH);
  digitalWrite(GATE_LED, HIGH);

 }


void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  liveNoteCount--;
  
  if (liveNoteCount <= 0) {
    digitalWrite(GATE_PIN, LOW);
    digitalWrite(GATE_LED, LOW);
  }
}


// these hold the current settings
byte envelopeLevel, cutoff, resonance, accent, sawLevel, squareLevel, slide, decay;

void handleControlChange(byte channel, byte number, byte value)
{  
  int scaledValue = int(value) << 1;
  
  switch (number) {

    case CUTOFF_CTRL:
      analogWrite(CUTOFF_PIN, scaledValue);
      cutoff = value;
      break;

    case ENV_MOD_CTRL:
      analogWrite(ENV_MOD_PIN, scaledValue);
      envelopeLevel = value;
      break;

    case SAW_CTRL:
      analogWrite(SAW_PIN, scaledValue);
      sawLevel = value;
      break;

    case SQR_CTRL:
      analogWrite(SQR_PIN, scaledValue);
      squareLevel = value;
      break;

    case DECAY_CTRL:
      analogWrite(DECAY_PIN, 255 - scaledValue);
      decay = value;
      break;

    case ACCENT_CTRL:
      analogWrite(ACCENT_PIN, scaledValue);
      accent = value;
      break;

    case SLIDE_CTRL:
      if (value >= 64) {
        digitalWrite(SLIDE_IN_PIN, HIGH);
        digitalWrite(SLIDE_OUT_PIN, LOW);
      } else {
        digitalWrite(SLIDE_IN_PIN, LOW);
        digitalWrite(SLIDE_OUT_PIN, HIGH);
      }
      slide = value;
      break;

    case RES_CTRL:
      ResonancePot.setValue(255-scaledValue, 255-scaledValue, 0);
      resonance = value;
      break;
      
    case ALL_NOTES_OFF:
      liveNoteCount = 0;
      handlePitchBend(deviceID, 0);
      digitalWrite(GATE_PIN, LOW);
      digitalWrite(GATE_LED, LOW);
      break;
  }

}


void handlePitchBend(byte channel, int bend)
{
  pitchbendOffset = bend >> 4;

  PitchDac.setValue(baseNoteFrequency + pitchbendOffset);
}


void handleSystemExclusive(byte message[], unsigned size) {
  
  if (message[1] != 0x77) return;      // manufacturer ID
  if (message[2] != 33) return;        // model ID
  if (message[3] != deviceID) return;  // device ID as set with trim pot

  
  byte param = message[4];

  switch (message[4]) {
    
    case 0:
      setMidiChannel(message[5]);
      break;
    
    case 1:
      sendPatchDump();
      break;

    case 2:
      receivePatchDump();
      break;
      
    default:
      break;
  }

}


void sendPatchDump() {
  byte sysexArray[] = { 0xf0, 0x77, 0x33, deviceID, 0x02, 0,0,0,0,0,0,0,0, 0xf7 };

  int paramByte = 5;

  sysexArray[paramByte++] = envelopeLevel;
  sysexArray[paramByte++] = resonance;
  sysexArray[paramByte++] = accent;
  sysexArray[paramByte++] = slide;
  
  sysexArray[paramByte++] = sawLevel;
  sysexArray[paramByte++] = squareLevel;
  sysexArray[paramByte++] = decay;
  sysexArray[paramByte++] = cutoff;

  MIDI.sendSysEx(14, sysexArray, true);
}

void receivePatchDump() {
  
}


void setMidiChannel(byte newChannel) {
  
  MIDI.begin(newChannel % 17);
  playScale(newChannel % 17);
  
}

// -----------------------------------------------------------------------------

void setup()
{
    int channelSpan = 1024 / 16;
    int channelInput = analogRead(0);
    deviceID = channelInput / channelSpan;
    
    pinMode(GATE_PIN, OUTPUT);
    digitalWrite(GATE_PIN, LOW);
    pinMode(GATE_LED, OUTPUT);
    digitalWrite(GATE_LED, LOW);


    digitalWrite(SLIDE_IN_PIN, OUTPUT);
    digitalWrite(SLIDE_IN_PIN, LOW);
    digitalWrite(SLIDE_OUT_PIN, OUTPUT);
    digitalWrite(SLIDE_OUT_PIN, HIGH);
    slide = 0;

    pinMode(ENV_MOD_PIN, OUTPUT);
    digitalWrite(ENV_MOD_PIN, HIGH);
    envelopeLevel = 127;

    pinMode(SAW_PIN, OUTPUT);
    digitalWrite(SAW_PIN, HIGH);
    sawLevel = 127;

    pinMode(SQR_PIN, OUTPUT);
    digitalWrite(SQR_PIN, LOW);
    squareLevel = 0;

    pinMode(CUTOFF_PIN, OUTPUT);
    digitalWrite(CUTOFF_PIN, HIGH);
    cutoff = 127;

    pinMode(DECAY_PIN, OUTPUT);
    digitalWrite(DECAY_PIN, LOW);
    decay = 0;
    
    pinMode(ACCENT_PIN, OUTPUT);
    digitalWrite(ACCENT_PIN, LOW);
    accent = 0;

    TCCR0B = (TCCR0B & 0b11111000) | 0x01;
    TCCR1B = (TCCR1B & 0b11111000) | 0x01;
    TCCR2B = (TCCR2B & 0b11111000) | 0x01;
 
    delay(1000);

    playScale(deviceID);

    // calibrate 8V
    baseNoteFrequency = (108 - 12) * 42;
    PitchDac.setValue(baseNoteFrequency);
    
    // calibrate full cutoff
    CutoffDac.setValue(32 * 127);
    cutoff = 127;

    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandlePitchBend(handlePitchBend);
    MIDI.setHandleControlChange(handleControlChange);

    MIDI.setHandleSystemExclusive(handleSystemExclusive);
    
    MIDI.begin(deviceID);
    MIDI.turnThruOn();
}


void playScale(int channel) {

  int note = 60;

  for (int i=0; i<channel; i++) {

      handleNoteOn(channel, note, 100);
      delay(5000);
      handleNoteOff(channel, note, 100);
      delay(5000);
      note++;
  }

}

int resControllerValue, lastResControllerValue, resPotValue;
float resPotRecentAvg = 0;

void loop()
{
  resPotValue = analogRead(RES_POT_PIN);
  resPotRecentAvg = .2 * (float)resPotValue + .8 * resPotRecentAvg;
  
  resControllerValue = 127 - ((int)resPotRecentAvg >> 3);

  if (abs(lastResControllerValue - resControllerValue) > 1) {
    lastResControllerValue = resControllerValue;
    handleControlChange(deviceID, RES_CTRL, resControllerValue);
    resonance = resControllerValue;
    MIDI.sendControlChange(RES_CTRL, resControllerValue, deviceID);
  }


  MIDI.read();
}

