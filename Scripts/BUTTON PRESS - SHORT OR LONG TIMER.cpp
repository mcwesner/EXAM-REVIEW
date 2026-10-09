#include <Arduino.h>

#define BUTTON_PIN 21
#define DEBOUNCE_MS 100
#define LONG_PRESS_MS 2000

int previousRaw = HIGH;
int stableButton = HIGH;
uint32_t lastRawChange = 0;

uint32_t pressStart = 0;
bool havePress = false;

void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  previousRaw = digitalRead(BUTTON_PIN);
  lastRawChange = millis();
}

void loop()
{
  uint32_t now = millis();
  int raw = digitalRead(BUTTON_PIN);

  //DEBOUNCE THE BUTTON
  if (raw != previousRaw)
  {
    previousRaw = raw;
    lastRawChange = now;
  }

  if (raw != stableButton &&
      now - lastRawChange >= DEBOUNCE_MS)
  {
    stableButton = raw;

    //PRESS: START TIMING
    if (stableButton == LOW)
    {
      pressStart = now;
      havePress = true;
    }

    // RELEASE: CHOOSE ONE ACTION
    else if (havePress)
    {
      uint32_t heldMs = now - pressStart;
      havePress = false;

      if (heldMs >= LONG_PRESS_MS)
      {
        // ACTION A
        Serial.println("Long press");
      }
      else
      {
        // ACTION B
        Serial.println("Short press");
      }
    }
  }
}