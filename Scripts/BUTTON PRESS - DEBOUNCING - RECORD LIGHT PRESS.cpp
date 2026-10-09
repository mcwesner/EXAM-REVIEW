#include <Arduino.h>

#define BUTTON_PIN 21
#define LED_PIN 2
#define DEBOUNCE_MS 100
#define QUIET_MS 5000
#define MAX_STEPS 100

int previousRaw = HIGH;
int stableButton = HIGH;
uint32_t lastRawChange = 0;

uint32_t durations[MAX_STEPS];
bool ledStates[MAX_STEPS];

int stepCount = 0;
int playIndex = 0;

bool recording = false;
bool playing = false;

uint32_t stateStart = 0;
uint32_t playStart = 0;

// SAVE ONE TIMING INTERVAL
bool saveStep(uint32_t duration, bool ledOn)
{
  if (stepCount >= MAX_STEPS)
  {
    recording = false;
    playing = false;
    stepCount = 0;
    digitalWrite(LED_PIN, LOW);
    return false;
  }

  durations[stepCount] = duration;
  ledStates[stepCount] = ledOn;
  stepCount++;
  return true;
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

  //DETECT RAW INPUT CHANGES
  if (raw != previousRaw)
  {
    previousRaw = raw;
    lastRawChange = now;
  }

  // ACCEPT A DEBOUNCED PRESS OR RELEASE
  if (raw != stableButton &&
      now - lastRawChange >= DEBOUNCE_MS)
  {
    bool wasPressed = (stableButton == LOW);
    stableButton = raw;

    // First press, or a press during playback.
    if (stableButton == LOW && !recording)
    {
      playing = false;
      recording = true;
      stepCount = 0;
      stateStart = now;
      digitalWrite(LED_PIN, LOW);
    }
    // Save the duration of the state that just ended.
    else if (recording)
    {
      saveStep(now - stateStart, wasPressed);
      stateStart = now;
    }
  }

  //START PLAYBACK AFTER 5 SECONDS RELEASED
  if (recording && stableButton == HIGH && raw == HIGH &&
      now - stateStart >= QUIET_MS)
  {
    if (saveStep(QUIET_MS, false))
    {
      recording = false;
      playing = true;
      playIndex = 0;
      playStart = now;
      digitalWrite(LED_PIN, ledStates[playIndex]);
    }
  }
  
  // PLAY AND REPEAT THE PATTERN
  if (playing && now - playStart >= durations[playIndex])
  {
    playIndex++;

    if (playIndex >= stepCount)
    {
      playIndex = 0;
    }

    playStart = now;
    digitalWrite(LED_PIN, ledStates[playIndex]);
  }
}