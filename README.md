# MIDI interface for x0x heart
Atmega328 MIDI interface for the x0x heart http://www.openmusiclabs.com/projects/x0x-heart/

### MIDI Implementation

* NOTE ON/OFF

   The gate pins of the eurorack connector are raised to 5V during the note. The "CV" pins of the eurorack connector are set to 1V/oct pitch control voltage. A pitch scale trimpot can be used to tune the x0x.

Controller # | Function
--- | ---   
1 | Envelope
11 | Accent
65 | Slide above 64 / No slide below 64
70 | Saw level
71 | Square level
72 | Decay
74 | VCF Cutoff
2 | Resonance (coming soon)


