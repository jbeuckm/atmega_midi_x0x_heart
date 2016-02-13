#include <MIDI.h>
#include "AH_MCP4922.h"

#define GATE_PIN 4

#define ENV_MOD_PIN 3
#define ENV_MOD_CTRL

#define CUTOFF_PIN 5
#define CUTOFF_CTRL

#define SAW_PIN 6
#define SAW_CTRL
#define SQR_PIN 9
#define SQR_CTRL

#define SLIDE_IN_PIN 13
#define SLIDE_OUT_PIN 12
#define SLIDE_CTRL

#define DECAY_PIN 11
#define DECAY_CTRL

#define ACCENT_PIN 10
#define ACCENT_CTRL


AH_MCP4922 PitchDac(A1,A2,A3,LOW,LOW);
AH_MCP4922 CutoffDac(A1,A2,A3,HIGH,LOW);

int liveNoteCount = 0;
int pitchbendOffset = 0;
int baseNoteFrequency;

MIDI_CREATE_DEFAULT_INSTANCE();

byte selectedChannel;

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  else if (channel != selectedChannel) {
    return;
  }
  
  liveNoteCount++;
  
  baseNoteFrequency = (pitch - 12) * 42;
  PitchDac.setValue(baseNoteFrequency + pitchbendOffset);
  AnalogOutput2.setValue(velocity * 32);

  digitalWrite(GATE_PIN, HIGH);
  digitalWrite(LED, HIGH);
  analogWrite(VELOCITY_PIN, 2 * velocity);
 }

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  if (channel != selectedChannel) {
    return;
  }
  liveNoteCount--;
  
  if (liveNoteCount == 0) {
    digitalWrite(GATE_PIN, LOW);
    digitalWrite(LED, LOW);
    analogWrite(VELOCITY_PIN, 0);
  }
}


void handleControlChange(byte channel, byte number, byte value)
{
  if (channel != selectedChannel) {
    return;
  }

}


void handlePitchBend(byte channel, int bend)
{
  pitchbendOffset = bend >> 4;

  PitchDac.setValue(baseNoteFrequency + pitchbendOffset);
}


// -----------------------------------------------------------------------------

void setup()
{
    int channelSpan = 1024 / 16;
    int channelInput = analogRead(0);
    selectedChannel = channelInput / channelSpan;

    Serial.begin(115200);
    Serial.println(channelInput);
    Serial.println(selectedChannel);
    
    pinMode(LED, OUTPUT);
    pinMode(GATE_PIN, OUTPUT);
    digitalWrite(GATE_PIN, LOW);
    digitalWrite(LED, LOW);

    delay(1000);

    playScale(selectedChannel);

    // calibrate 8V
    baseNoteFrequency = (108 - 12) * 42;
    AnalogOutput1.setValue(baseNoteFrequency);
    // calibrate full velocity
    AnalogOutput2.setValue(32 * 127);

    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandlePitchBend(handlePitchBend);
    MIDI.begin(selectedChannel);
}


void playScale(int channel) {

  int note = 60;

  for (int i=0; i<channel; i++) {

      handleNoteOn(channel, note, 100);
      delay(100);
      handleNoteOff(channel, note, 100);
      delay(100);
      note++;
  }

}


void loop()
{
    MIDI.read();
}

