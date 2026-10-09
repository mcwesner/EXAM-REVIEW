#include <Arduino.h>

#define SENSOR_PIN 21

uint32_t riseStart = 0;
uint32_t highUs = 0;
bool haveRise = false;
bool haveFall = false;

volatile uint32_t periodUs = 0;
volatile uint32_t highTimeUs = 0;
volatile bool newCycle = false;

uint32_t previousPeriod = 0;
uint32_t previousHigh = 0;

void IRAM_ATTR onChange()
{
  uint32_t now = micros();

  if (digitalRead(SENSOR_PIN) == HIGH)
  {
    if (haveRise && haveFall)
    {
      periodUs = now - riseStart;
      highTimeUs = highUs;
      newCycle = true;
    }

    riseStart = now;
    haveRise = true;
    haveFall = false;
  }
  else if (haveRise)
  {
    highUs = now - riseStart;
    haveFall = true;
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), onChange, CHANGE);
}

void loop()
{
  noInterrupts();
  bool ready = newCycle;
  uint32_t periodNow = periodUs;
  uint32_t highNow = highTimeUs;
  newCycle = false;
  interrupts();

  if (ready && periodNow > 0 &&
      (periodNow != previousPeriod || highNow != previousHigh))
  {
    float frequencyHz = 1000000.0f / periodNow;
    float dutyPercent = 100.0f * highNow / periodNow;

    // Change the action using frequencyHz and dutyPercent.
    Serial.printf(
      "Frequency: %.2f Hz, Duty: %.1f%%\n",
      frequencyHz,
      dutyPercent
    );

    previousPeriod = periodNow;
    previousHigh = highNow;
  }
}