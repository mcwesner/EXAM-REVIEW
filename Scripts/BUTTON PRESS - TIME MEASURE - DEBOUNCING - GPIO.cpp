#include <Arduino.h>


#define BUTTON_PIN 21
#define DEBOUNCE_MS 100


int previousRaw = HIGH;
int stableButton = HIGH;
uint32_t lastRawChange = 0;


uint32_t pressStart = 0;
bool havePress = false;

//ACTION ON ACCEPTED PRESS OR RELEASE
void onButtonChange(uint32_t now)
{
  // Button pressed.
  if (stableButton == LOW)
  {
    pressStart = now;
    havePress = true;
  }
  // Button released after a recorded press.
  else if (havePress)
  {
    uint32_t heldMs = now - pressStart;

    Serial.printf(
      "Held: %lu ms\n",
      (unsigned long)heldMs
    );

    havePress = false;
  }
}


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

  //DETECT RAW INPUT CHANGES
  if (raw != previousRaw)
  {
    previousRaw = raw;
    lastRawChange = now;
  }

  // ACCEPT A DEBOUNCED CHANGE
  if (raw != stableButton &&
      now - lastRawChange >= DEBOUNCE_MS)
  {
    stableButton = raw;
    onButtonChange(now);
  }
}