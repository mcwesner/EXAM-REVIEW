#include <Arduino.h>

#define POT_PIN 34
#define LED_PIN 2

int previousDuty = -1;

void setup()
{
  analogReadResolution(12);
  pinMode(POT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  int reading = analogRead(POT_PIN);
  int duty = reading * 255 / 4095;

  if (duty != previousDuty)
  {
    // Change the action using the scaled reading.
    analogWrite(LED_PIN, duty);
    previousDuty = duty;
  }
}