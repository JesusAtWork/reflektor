#ifndef Display16_h
#define Display16_h

#include <Arduino.h>

class Display16 {
  int _display_len;
  int _enable_pin;
  int _clock_pin;
  int _data_pin;

public:
  Display16(int display_len, int enable_pin, int clock_pin, int data_pin) {
    _display_len = display_len;
    _enable_pin = enable_pin;
    _clock_pin = clock_pin;
    _data_pin = data_pin;

    pinMode(_enable_pin, OUTPUT);
    pinMode(_clock_pin, OUTPUT);
    pinMode(_data_pin, OUTPUT);
  }

  void show(char* string);
};

#endif
