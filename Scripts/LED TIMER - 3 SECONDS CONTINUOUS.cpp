#include <Arduino.h>

#define SENSOR_PIN 21
#define LED_PIN 2
#define ACTIVE_MS 3000

volatile bool eventPending = false;
bool running = false;
uint32_t actionStart = 0;

void IRAM_ATTR onEvent()
{
  eventPending = true;
}

void setup()
{
  pinMode(SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  attachInterrupt(
    digitalPinToInterrupt(SENSOR_PIN),
    onEvent,
    RISING
  );
}

void loop()
{
  uint32_t now = millis();

  // COPY AND CLEAR THE EVENT FLAG
  noInterrupts();
  bool triggered = eventPending;
  eventPending = false;
  interrupts();

  // ACTION A: START OR RESTART
  if (triggered)
  {
    actionStart = now;
    running = true;

    // action when a sensor event arrives.
    digitalWrite(LED_PIN, HIGH);
  }

  // ACTION B: FINISH AFTER THREE SECONDS
  if (running && now - actionStart >= ACTIVE_MS)
  {
    running = false;

    // action when the timer ends.
    digitalWrite(LED_PIN, LOW);
  }
}