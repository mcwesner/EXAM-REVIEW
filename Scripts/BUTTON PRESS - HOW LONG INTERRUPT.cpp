#include <Arduino.h>

#define INPUT_PIN 21

uint32_t startUs = 0;
bool started = false;

volatile uint32_t highTimeUs = 0;
uint32_t previousDuration = 0;

void IRAM_ATTR onEvent()
{
  uint32_t now = micros();

  // Rising edge: start timing.
  if (digitalRead(INPUT_PIN) == HIGH)
  {
    startUs = now;
    started = true;
  }
  // Falling edge: finish timing.
  else if (started)
  {
    highTimeUs = now - startUs;
    started = false;
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(INPUT_PIN, INPUT);

  attachInterrupt(
    digitalPinToInterrupt(INPUT_PIN),
    onEvent,
    CHANGE
  );
}

void loop()
{
  // COPY THE INTERRUPT MEASUREMENT
  noInterrupts();
  uint32_t durationNow = highTimeUs;
  interrupts();

  // PRINT ONLY WHEN THE DURATION CHANGES
  if (durationNow != previousDuration)
  {
    Serial.printf(
      "HIGH duration: %lu us\n",
      (unsigned long)durationNow
    );

    previousDuration = durationNow;
  }
}