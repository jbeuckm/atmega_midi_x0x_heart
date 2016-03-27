/*
  Library for controlling the Dallas Semiconductor Dual Digital Pot
*/
#ifndef DS1267_h
#define DS1267_h

#include "Arduino.h"

class DS1267
{

  public:
    DS1267(int clkPin, int dqPin, int resetPin);
    void setValue(byte potNumber, byte value);

  private:
    int _clkPin, _dqPin, _resetPin;
    
};

#endif