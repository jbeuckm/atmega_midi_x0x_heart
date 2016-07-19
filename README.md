# MIDI interface for x0x heart
Atmega328 MIDI interface for the x0x heart http://www.openmusiclabs.com/projects/x0x-heart/

#### NOTE ON/OFF

   The gate pins of the eurorack connector are raised to 5V during the note. The "CV" pins of the eurorack connector are set to 1V/oct pitch control voltage. A pitch scale trimpot can be used to tune the x0x.

### Continuous Controllers

MIDI CC | Function
--- | ---   
1 | Envelope
2 | Resonance
11 | Accent
65 | Slide above 64 / No slide below 64
70 | Saw level
71 | Square level
72 | Decay
74 | VCF Cutoff

### System Exclusive

MIDI Sysex messages for this device begin with a four-byte prefix of the Sysex status byte, manufacturer ID, model ID and device ID. 

`F0` sysex start

`77 33` manufacturer ID, model ID

`xx` device ID as set by trim pot

--

At startup, the MIDI channel of the device is the same as the device ID set with the trim pot.

A seven-byte sysex message can be used to change the channel of the interface.

`F0 77 33 xx` sysex prefix

`00` select MIDI channel parameter

`xx` new MIDI channel (0 for omni) 

`F7` end of message

--

A six-byte sysex message can be used to request a dump of the current voice parameters.

`F0 77 33 xx` sysex prefix

`01` patch dump request

`F7` end of message

The interface will then send a patch dump sysex message that encodes the current voice parameter values.

`F0 77 33 xx` sysex prefix

`02` patch dump

`ee rr aa pp ss qq dd ff`

envelope, resonance, accent, slide, saw, square, decay, cutoff (eight bytes)

`F7` end of message

This dumped patch data can be used to set all voice parameters by sending it back to the interface.
