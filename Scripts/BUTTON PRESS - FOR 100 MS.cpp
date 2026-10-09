#include <Arduino.h>

#define BUTTON_PIN 21
#define DEBOUNCE_MS 100

// DEBOUNCING VARIABLES
int previousRaw;
int stableButton;
uint32_t lastRawChange;

// SETUP 
void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  previousRaw = digitalRead(BUTTON_PIN);
  stableButton = previousRaw;
  lastRawChange = millis();
}

void loop()
{
  uint32_t now = millis();
  int raw = digitalRead(BUTTON_PIN);

  // DETECT RAW INPUT CHANGES
  if (raw != previousRaw)
  {
    previousRaw = raw;
    lastRawChange = now;
  }

  //ACCEPT A STABLE INPUT CHANGE
  if (raw != stableButton &&
      now - lastRawChange >= DEBOUNCE_MS)
  {
    stableButton = raw;

    //ACTION ON PRESS
    if (stableButton == LOW)
    {
      Serial.println("Pressed");
    }
    //  ACTION ON RELEASE 
    else
    {
      Serial.println("Released");
    }
  }
}