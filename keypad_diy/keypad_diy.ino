
#include "keypad.h"


void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);

  keypadBegin();
}

void loop() {
  
  keypadUpdate();


  if (keypadIsPressedEvent(0, 0) == true)
  {
    if (keypadIsPressed(3, 0) == true)
    {
      Serial.print("ctrl + ");
    }        
    if (keypadIsPressed(2, 0) == true)
    {
      Serial.print("shift + ");
    }

    Serial.println("1");
  }  
}
