#ifndef _KEYPAD_H_
#define _KEYPAD_H_


#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif


#define KEYPAD_ROWS       4
#define KEYPAD_COLS       4

#define KEYPAD_NUM_MAX    16





void keypadBegin(void);
void keypadUpdate(void);
void keypadSetRows(uint8_t ch);
uint8_t keypadGetRows(void);


bool keypadIsPressed(uint8_t row, uint8_t col);
bool keypadIsPressedEvent(uint8_t row, uint8_t col);

bool keypadIsReleased(uint8_t row, uint8_t col);
bool keypadIsReleasedEvent(uint8_t row, uint8_t col);

#endif

