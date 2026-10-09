#include <Arduino.h>


#define SENSOR_A 21
#define SENSOR_B 22
#define DISTANCE_METERS 0.5f

uint32_t startUs = 0;
bool waitingForB = false;

volatile uint32_t travelUs = 0;
volatile bool newMeasurement = false;

//SENSOR A INTERRUPT: START TIMING
void IRAM_ATTR onA()
{
  if (!waitingForB)
  {
    startUs = micros();
    waitingForB = true;
  }
}

// SENSOR B INTERRUPT: FINISH TIMING
void IRAM_ATTR onB()
{
  if (waitingForB)
  {
    travelUs = micros() - startUs;
    waitingForB = false;
    newMeasurement = true;
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(SENSOR_A, INPUT);
  pinMode(SENSOR_B, INPUT);

  attachInterrupt(digitalPinToInterrupt(SENSOR_A), onA, RISING);
  attachInterrupt(digitalPinToInterrupt(SENSOR_B), onB, RISING);
}

void loop()
{
  noInterrupts();
  bool ready = newMeasurement;
  uint32_t elapsedUs = travelUs;
  newMeasurement = false;
  interrupts();

  //CALCULATE SPEED AND ACT
  if (ready && elapsedUs > 0)
  {
    float speedMps =
      DISTANCE_METERS * 1000000.0f / elapsedUs;

    
    Serial.printf("Speed: %.3f m/s\n", speedMps);
  }
}