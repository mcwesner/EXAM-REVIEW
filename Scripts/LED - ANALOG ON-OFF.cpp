#include <Arduino.h>

#define SENSOR_PIN 34
#define LED_PIN 2
#define ON_THRESHOLD 2500
#define OFF_THRESHOLD 2000

bool ledOn = false;

void setup()
{
  Serial.begin(9600);
  analogReadResolution(12);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop()
{
  int reading = analogRead(SENSOR_PIN);
  bool nextState = ledOn;

  if (!ledOn && reading >= ON_THRESHOLD)
  {
    nextState = true;
  }
  else if (ledOn && reading <= OFF_THRESHOLD)
  {
    nextState = false;
  }

  if (nextState != ledOn)
  {
    ledOn = nextState;

    // Change the action when the state changes.
    digitalWrite(LED_PIN, ledOn);
    Serial.printf("LED: %s\n", ledOn ? "ON" : "OFF");
  }
}