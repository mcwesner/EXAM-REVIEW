#include <Arduino.h>

#define BUTTON_PIN 21
#define DEBOUNCE_MS 100

int previousRaw = HIGH;
int stableButton = HIGH;
uint32_t lastRawChange = 0;

uint32_t releaseStart = 0;
bool haveRelease = false;

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

    // RELEASE: START TIMING THE GAP
    if (stableButton == HIGH)
    {
      releaseStart = now;
      haveRelease = true;
    }

    //NEXT PRESS: MEASURE AND ACT
    else if (haveRelease)
    {
      uint32_t gapMs = now - releaseStart;

      // action using the measured gap
      Serial.printf(
        "Released gap: %lu ms\n",
        (unsigned long)gapMs
      );

      haveRelease = false;
    }
  }
}