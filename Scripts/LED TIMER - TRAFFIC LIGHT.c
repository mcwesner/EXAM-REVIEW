#include <Arduino.h>

#define RED_PIN 18
#define GREEN_PIN 19
#define YELLOW_PIN 21

#define RED_MS 3000
#define GREEN_MS 5000
#define YELLOW_MS 1000

int state = 0;
uint32_t stateStart = 0;

void setup()
{
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);

  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);

  stateStart = millis();
}

void loop()
{
  uint32_t now = millis();

  //SELECT THE CURRENT STAGE'S DURATION
  uint32_t waitMs;

  if (state == 0)
  {
    waitMs = RED_MS;
  }
  else if (state == 1)
  {
    waitMs = GREEN_MS;
  }
  else
  {
    waitMs = YELLOW_MS;
  }

  //ADVANCE WHEN THE DURATION HAS PASSED
  if (now - stateStart >= waitMs)
  {
    state = (state + 1) % 3;
    stateStart = now;
  }

  // SET OUTPUTS FOR THE CURRENT STAGE
  digitalWrite(RED_PIN, state == 0);
  digitalWrite(GREEN_PIN, state == 1);
  digitalWrite(YELLOW_PIN, state == 2);

}