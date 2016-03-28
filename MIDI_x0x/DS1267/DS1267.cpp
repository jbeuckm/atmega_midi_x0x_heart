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


void DS1267::setValue(byte pot1value, byte pot2value, byte stackBit) {

    digitalWrite(_resetPin, HIGH);
    delayMicroseconds(1);

    if (stackBit){
      digitalWrite(_dataPin, HIGH);
    }
    else{
      digitalWrite(_dataPin, LOW);
    }

    delayMicroseconds(1);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(1);
    digitalWrite(_clkPin, LOW);
    delayMicroseconds(1);

    
  for (mask = B10000000; mask>0; mask >>= 1) {

    if (pot1value & mask){
      digitalWrite(_dataPin, HIGH);
    }
    else{
      digitalWrite(_dataPin, LOW);
    }

    delayMicroseconds(1);
	digitalWrite(_clkPin, HIGH);
    delayMicroseconds(1);
	digitalWrite(_clkPin, LOW);
    delayMicroseconds(1);

  }


  for (mask = B10000000; mask>0; mask >>= 1) {

    if (pot2value & mask){
      digitalWrite(_dataPin, HIGH);
    }
    else{
      digitalWrite(_dataPin, LOW);
    }

    delayMicroseconds(1);
	digitalWrite(_clkPin, HIGH);
    delayMicroseconds(1);
	digitalWrite(_clkPin, LOW);
    delayMicroseconds(1);

  }

	digitalWrite(_resetPin, LOW);
    delayMicroseconds(1);    
}

