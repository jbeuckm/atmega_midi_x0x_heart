# MIDI interface for x0x heart
Atmega328 MIDI interface for the x0x heart http://www.openmusiclabs.com/projects/x0x-heart/

### MIDI Implementation

* NOTE ON/OFF

   The gate pins of the eurorack connector are raised to 5V during the note. The "CV" pins of the eurorack connector are set to 1V/oct pitch control voltage. A pitch scale trimpot can be used to tune the x0x.
   
* Controller 1 --> Envelope
* Controller 11 --> Accent
* Controller 65 --> Slide above 64 / No slide below 64
* Controller 70 --> Saw level
* Controller 71 --> Square level
* Controller 72 --> Decay
* Controller 74 --> VCF Cutoff
 