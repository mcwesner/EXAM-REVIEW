#include <Arduino.h>

#define LED_A 2
#define LED_B 18
#define INTERVAL_A 500
#define INTERVAL_B 1000

uint32_t lastA = 0;
bool stateA = false;

uint32_t lastB = 0;
bool stateB = false;

void setup()
{
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);

  digitalWrite(LED_A, LOW);
  digitalWrite(LED_B, LOW);

  lastA = millis();
  lastB = lastA;
}

void loop()
{
  uint32_t now = millis();

  //TIMER A 
  if (now - lastA >= INTERVAL_A)
  {
    lastA = now;

    stateA = !stateA;
    digitalWrite(LED_A, stateA);
  }

  // TIMER B
  if (now - lastB >= INTERVAL_B)
  {
    lastB = now;

    stateB = !stateB;
    digitalWrite(LED_B, stateB);
  }

  
}