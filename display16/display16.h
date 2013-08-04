#ifndef Display16_h
#define Display16_h

#include <Arduino.h>

class Display16 {
  byte _display_len;
  byte _enable_pin;
  byte _clock_pin;
  byte _data_pin;

public:
  Display16(byte display_len, byte enable_pin, byte clock_pin, byte data_pin) {
    _display_len = display_len;
    _enable_pin = enable_pin;
    _clock_pin = clock_pin;
    _data_pin = data_pin;

    pinMode(_enable_pin, OUTPUT);
    pinMode(_clock_pin, OUTPUT);
    pinMode(_data_pin, OUTPUT);
  }

  void show(const char* string);
};

#endif
