#include <Arduino.h>

#define LED_PIN 2

bool ledOn = false;

void setup()
{
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop()
{
  if (Serial.available() > 0)
  {
    char command = Serial.read();
    bool recognized = true;

    if (command == '1')
    {
      ledOn = true;
    }
    else if (command == '0')
    {
      ledOn = false;
    }
    else if (command == 't')
    {
      ledOn = !ledOn;
    }
    else
    {
      recognized = false;
    }

    if (recognized)
    {
      // Change the action for a recognized command.
      digitalWrite(LED_PIN, ledOn);
      Serial.printf("LED: %s\n", ledOn ? "ON" : "OFF");
    }
  }
}