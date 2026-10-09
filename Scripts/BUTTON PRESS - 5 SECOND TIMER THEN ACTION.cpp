#include <Arduino.h>

#define BUTTON_PIN 21
#define DEBOUNCE_MS 100
#define TIMEOUT_MS 5000

int previousRaw;
int stableButton;
uint32_t lastRawChange = 0;

uint32_t releaseStart = 0;
bool timeoutDone = false;

void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  previousRaw = digitalRead(BUTTON_PIN);
  stableButton = previousRaw;

  lastRawChange = millis();
  releaseStart = lastRawChange;
}

void loop()
{
  uint32_t now = millis();
  int raw = digitalRead(BUTTON_PIN);

  //DETECT RAW INPUT CHANGES
  if (raw != previousRaw)
  {
    previousRaw = raw;
    lastRawChange = now;
  }

  //ACCEPT A DEBOUNCED CHANGE
  if (raw != stableButton &&
      now - lastRawChange >= DEBOUNCE_MS)
  {
    stableButton = raw;
    timeoutDone = false;

    // Start the countdown when released.
    if (stableButton == HIGH)
    {
      releaseStart = now;
    }
  }

  // ONE ACTION AFTER FIVE SECONDS RELEASED
  if (stableButton == HIGH &&
      raw == HIGH &&
      !timeoutDone &&
      now - releaseStart >= TIMEOUT_MS)
  {
    // Replace this with the required action.
    Serial.println("Button released for 5 seconds");

    timeoutDone = true;
  }
}