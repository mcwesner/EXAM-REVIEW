#include <Arduino.h>

#define SENSOR_PIN 21

int previousState;

void setup()
{
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);

  previousState = digitalRead(SENSOR_PIN);
}

void loop()
{
  //READ THE CURRENT STATE
  int currentState = digitalRead(SENSOR_PIN);

  //ACT ONLY WHEN THE STATE CHANGES
  if (currentState != previousState)
  {
    if (currentState == HIGH)
    {
      Serial.println("HIGH");
    }
    else
    {
      Serial.println("LOW");
    }

    //REMEMBER THE NEW STATE
    previousState = currentState;
  }
}