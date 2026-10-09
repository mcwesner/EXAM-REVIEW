#include <Arduino.h>

#define SENSOR_PIN 21
#define RESET_PIN 22
#define DEBOUNCE_MS 100

volatile uint32_t count = 0;
uint32_t previousCount = 0;

int previousRaw = HIGH;
int stableButton = HIGH;
uint32_t lastRawChange = 0;

void IRAM_ATTR onProduct()
{
  count++;
}

void setup()
{
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(RESET_PIN, INPUT_PULLUP);

  previousRaw = digitalRead(RESET_PIN);
  lastRawChange = millis();

  attachInterrupt(
    digitalPinToInterrupt(SENSOR_PIN),
    onProduct,
    RISING
  );
}

void loop()
{
  uint32_t now = millis();
  int raw = digitalRead(RESET_PIN);

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
      noInterrupts();
      count = 0;
      interrupts();
    }
  }

  noInterrupts();
  uint32_t countNow = count;
  interrupts();

  if (countNow != previousCount)
  {
    // Change the action when the total changes.
    Serial.printf("Total: %lu\n", (unsigned long)countNow);
    previousCount = countNow;
  }
}