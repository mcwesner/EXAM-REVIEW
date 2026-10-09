#include <Arduino.h>

#define BUTTON_PIN 21
#define LED_PIN 2
#define DEBOUNCE_MS 100

const uint32_t durations[] = {3000, 1000, 2000, 5000};
const int ledStates[] = {HIGH, LOW, HIGH, LOW};
const int STEP_COUNT = sizeof(durations) / sizeof(durations[0]);

int previousRaw = HIGH;
int stableButton = HIGH;
uint32_t lastRawChange = 0;

bool playing = false;
int playIndex = 0;
uint32_t stepStart = 0;

void runStepAction()
{
  // Change the action for the current step.
  digitalWrite(LED_PIN, ledStates[playIndex]);
}

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

  if (raw != previousRaw)
  {
    previousRaw = raw;
    lastRawChange = now;
  }

  if (raw != stableButton &&
      now - lastRawChange >= DEBOUNCE_MS)
  {
    stableButton = raw;

    if (stableButton == LOW)
    {
      playing = !playing;
      playIndex = 0;
      stepStart = now;

      if (playing)
      {
        runStepAction();
      }
      else
      {
        digitalWrite(LED_PIN, LOW);
      }
    }
  }

  if (playing && now - stepStart >= durations[playIndex])
  {
    playIndex = (playIndex + 1) % STEP_COUNT;
    stepStart = now;
    runStepAction();
  }
}