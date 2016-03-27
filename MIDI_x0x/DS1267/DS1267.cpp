#include "Arduino.h"
#include "DS1267.h"


DS1267::DS1267(int clkPin, int dqPin, int resetPin) {
    _clkPin = clkPin;
    pinMode(_clkPin, OUTPUT);
    _dqPin = dqPin;
    pinMode(_dqPin, OUTPUT);
    _resetPin = resetPin;
    pinMode(_resetPin, OUTPUT);

	digitalWrite(_resetPin, LOW);
	digitalWrite(_clkPin, LOW);
	digitalWrite(_dqPin, LOW);
}


void DS1267::setValue(byte potNumber, byte value) {

	digitalWrite(_ldacPin, LOW);
	digitalWrite(_clkPin, LOW);
	digitalWrite(_loadPin, HIGH);

	byte mask;

  for (mask = B100; mask>0; mask >>= 1) {

	digitalWrite(_clkPin, HIGH);

    if (dacNumber & mask){
      digitalWrite(_dataPin, HIGH);
    }
    else{
      digitalWrite(_dataPin, LOW);
    }

    delayMicroseconds(1);
	digitalWrite(_clkPin, LOW);
    delayMicroseconds(1);
  }


	// clock in the RNG bit (x2 output voltage)
	digitalWrite(_clkPin, HIGH);
	delayMicroseconds(1);
	digitalWrite(_dataPin, HIGH);
	delayMicroseconds(1);
	digitalWrite(_clkPin, LOW);
	delayMicroseconds(1);

    
  for (mask = B10000000; mask>0; mask >>= 1) {

	digitalWrite(_clkPin, HIGH);

    if (value & mask){
      digitalWrite(_dataPin, HIGH);
    }
    else{
      digitalWrite(_dataPin, LOW);
    }

    delayMicroseconds(1);
	digitalWrite(_clkPin, LOW);
    delayMicroseconds(1);

  }

      digitalWrite(_dataPin, LOW);
    delayMicroseconds(1);


	digitalWrite(_loadPin, LOW);
    delayMicroseconds(1);
	digitalWrite(_loadPin, HIGH);
    delayMicroseconds(1);
    
}

