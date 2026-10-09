#include <Arduino.h>

#define LED_PIN 2
#define INTERVAL_MS 500

uint32_t lastToggle = 0;
bool ledOn = false;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  lastToggle = millis();
}

void loop()
{
  uint32_t now = millis();

  // CHECK WHETHER THE INTERVAL HAS PASSED
  if (now - lastToggle >= INTERVAL_MS)
  {
    lastToggle = now;

    // REPEATED ACTION
    ledOn = !ledOn;
    digitalWrite(LED_PIN, ledOn);
  }

  // OTHER TASKS
}