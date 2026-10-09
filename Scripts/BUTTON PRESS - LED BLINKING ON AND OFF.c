#include <Arduino.h>

#define BUTTON_PIN 21
#define LED_PIN 2
#define DEBOUNCE_MS 100
#define INTERVAL_MS 500

int previousRaw = HIGH;
int stableButton = HIGH;
uint32_t lastRawChange = 0;

bool enabled = false;
bool ledOn = false;
uint32_t lastAction = 0;

void setup()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

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

    //PRESS: START OR STOP THE REPEATING TAS
    if (stableButton == LOW)
    {
      enabled = !enabled;
      lastAction = now;
      ledOn = false;

      // REPEATING ACTION
      digitalWrite(LED_PIN, LOW);
    }
  }

  //ACTION B: 
  if (enabled && now - lastAction >= INTERVAL_MS)
  {
    lastAction = now;

    // REPEATING ACTION
    ledOn = !ledOn;
    digitalWrite(LED_PIN, ledOn);
  }
}