#include <Arduino.h>

#define SENSOR_PIN 21
#define TIMEOUT_MS 5000

volatile uint32_t lastEventMs = 0;
volatile bool eventPending = false;
bool timeoutDone = false;

void IRAM_ATTR onEvent()
{
  lastEventMs = millis();
  eventPending = true;
}

void setup()
{
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
  lastEventMs = millis();

  attachInterrupt(
    digitalPinToInterrupt(SENSOR_PIN),
    onEvent,
    RISING
  );
}

void loop()
{
  //COPY THE INTERRUPT DATA
  noInterrupts();
  uint32_t now = millis();
  uint32_t lastEvent = lastEventMs;
  bool triggered = eventPending;
  eventPending = false;
  interrupts();

  // NEW EVENT: ALLOW ANOTHER TIMEOUT ACTION
  if (triggered)
  {
    timeoutDone = false;
  }

  // ACTION: ONCE AFTER FIVE SECONDS WITHOUT EVENTS
  if (!timeoutDone && now - lastEvent >= TIMEOUT_MS)
  {
    // action when sensor events stop
    Serial.println("No sensor events for 5 seconds");
    timeoutDone = true;
  }
}