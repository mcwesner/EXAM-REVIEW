#include <Arduino.h>

#define BUTTON_PIN 21
#define DEBOUNCE_MS 100
#define QUIET_MS 5000

int previousRaw = HIGH;
int stableButton = HIGH;
uint32_t lastRawChange = 0;

uint32_t pressCount = 0;
uint32_t releaseStart = 0;
bool collecting = false;

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

  // DEBOUNCE THE BUTTON
  if (raw != previousRaw)
  {
    previousRaw = raw;
    lastRawChange = now;
  }

  if (raw != stableButton &&
      now - lastRawChange >= DEBOUNCE_MS)
  {
    stableButton = raw;

    // PRESS: START A BURST OR ADD ONE PRESS
    if (stableButton == LOW)
    {
      if (!collecting)
      {
        collecting = true;
        pressCount = 0;
      }

      pressCount++;
    }

    //RELEASE: RESTART THE QUIET TIMER
    else if (collecting)
    {
      releaseStart = now;
    }
  }

  //ACTION: HANDLE THE COMPLETED BURST ONCE
  if (collecting &&
      stableButton == HIGH &&
      raw == HIGH &&
      now - releaseStart >= QUIET_MS)
  {
    //action using pressCount
    Serial.printf(
      "Presses in burst: %lu\n",
      (unsigned long)pressCount
    );

    collecting = false;
  }
}