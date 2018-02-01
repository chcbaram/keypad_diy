
#include "keypad.h"



static const uint8_t row_pins[KEYPAD_ROWS] = {A0, A1, A2, A3}; 
static const uint8_t col_pins[KEYPAD_COLS] = {10, 16, 14, 15};



typedef struct
{
  uint8_t state;

  bool pressed;
  bool pressed_event;
  bool released_event;
  uint32_t pre_time;
} keypad_info_t;

bool          keypad_pressed = false;
keypad_info_t keypad_info[KEYPAD_ROWS][KEYPAD_COLS];



static uint32_t keypadScan(void);
static bool keypadProcess(uint8_t row, uint8_t col, bool pressed);





void keypadBegin(void)
{
  for (int i=0; i<KEYPAD_ROWS; i++)
  {
    pinMode(row_pins[i], INPUT_PULLUP);
  }

  for (int i=0; i<KEYPAD_COLS; i++)
  {
    pinMode(col_pins[i], INPUT);
    digitalWrite(col_pins[i], LOW);
  }

  for (uint8_t row = 0; row < KEYPAD_ROWS; row++)
  {
    for (uint8_t col = 0; col < KEYPAD_COLS; col++)
    {      
      keypad_info[row][col].state = 0;
      keypad_info[row][col].pre_time = millis();
      keypad_info[row][col].pressed = false;
      keypad_info[row][col].pressed_event = false;
      keypad_info[row][col].released_event = false;
    }
  }  
}

void keypadUpdate(void)
{
  uint8_t row;
  uint8_t col;
  uint32_t key_data;
  bool pressed;
  bool pressed_any = false;


  key_data = keypadScan();


  for (row = 0; row < KEYPAD_ROWS; row++)
  {
    for (col = 0; col < KEYPAD_COLS; col++)
    {
      if (key_data & (1<<(row*KEYPAD_COLS + col)))
      {
        pressed = true;
      }
      else
      {
        pressed = false;
      }            
      pressed_any |= keypadProcess(row, col, pressed);
    }
  }

  keypad_pressed = pressed_any;
}

void keypadSetRows(uint8_t ch)
{
  for (int i=0; i<KEYPAD_COLS; i++)
  {
    pinMode(col_pins[i], INPUT);
  }  

  pinMode(col_pins[ch], OUTPUT);  
}

uint8_t keypadGetRows(void)
{
  uint8_t ret = 0;
  uint8_t pressed_bit;


  for (int i=0; i<KEYPAD_ROWS; i++)
  {
    pressed_bit = !digitalRead(row_pins[i]); 
    ret |= (pressed_bit<<i);
  }  

  return ret;
}

bool keypadIsPressed(uint8_t row, uint8_t col)
{
  return keypad_info[row][col].pressed;
}

bool keypadIsPressedEvent(uint8_t row, uint8_t col)
{
  bool ret;

  ret = keypad_info[row][col].pressed_event;

  keypad_info[row][col].pressed_event = false;

  return ret;
}

bool keypadIsReleased(uint8_t row, uint8_t col)
{
  return !keypad_info[row][col].pressed;
}

bool keypadIsReleasedEvent(uint8_t row, uint8_t col)
{
  bool ret;

  ret = keypad_info[row][col].released_event;

  keypad_info[row][col].released_event = false;

  return ret;
}

uint32_t keypadScan(void)
{
  uint32_t key_data = 0;


  for (uint32_t i=0; i<KEYPAD_ROWS; i++)
  {
    keypadSetRows(i);
    key_data |= keypadGetRows()<<(i*4);
  }  

  return key_data;
}

bool keypadProcess(uint8_t row, uint8_t col, bool pressed)
{
  bool ret = 0;
  keypad_info_t *p_info;


  p_info = &keypad_info[row][col];

  switch(p_info->state)
  {
    case 0:
      p_info->pressed = false;

      if (pressed == true)
      {
        p_info->pre_time = millis();      
        p_info->state = 1;
      }
      break;

    // 버튼 눌림 필터링 
    case 1:
      if (pressed == false)
      {
        p_info->state = 0;
      }
      if (millis()-p_info->pre_time >= 50)
      {
        p_info->state = 2;
        p_info->pressed = true;
        p_info->pressed_event = true;
      }
      break;

    // 버튼이 떨어질때까지 기다림 
    case 2:
      if (pressed == false)
      {
        p_info->pre_time = millis();      
        p_info->state = 3;        
      }      
      break;

    // 버튼 떨어짐 필터링 
    case 3:
      if (pressed == true)
      {
        p_info->state = 2;
      }
      if (millis()-p_info->pre_time >= 50)
      {
        p_info->state = 0;
        p_info->pressed = false;
        p_info->released_event = true;        
      }
      break;

    default:
      p_info->state = 0;
      break;      
  }

  return p_info->pressed;
}