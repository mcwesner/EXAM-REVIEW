#include <Arduino.h>

#define BEAM_PIN 21

volatile uint32_t boxCount = 0;
volatile uint32_t totalTime = 0;  // Microseconds BETWEEN products.

uint32_t currTime = 0;
bool havePreviousProduct = false;

uint32_t previousCount = 0;
uint32_t previousTime = 0;

//2. INTERRUPT: COUNT AND MEASURE TIME 
void IRAM_ATTR onRising()
{
  uint32_t now = micros();

  // COUNT SECTION: one rising edge means one product.
  boxCount++;

  // TIMING SECTION: the second product gives the first valid interval.
  if (havePreviousProduct)
  {
    totalTime = now - currTime;
  }
  currTime = now;
  havePreviousProduct = true;
}

//3. SETUP: SERIAL, INPUT, AND INTERRUPT
void setup()
{
  Serial.begin(9600);
  pinMode(BEAM_PIN, INPUT);
  attachInterrupt(BEAM_PIN, onRising, RISING);
}

void loop()
{
  //4. COPY THE INTERRUPT MEASUREMENTS
  noInterrupts();
  uint32_t countNow = boxCount;
  uint32_t timeNow = totalTime;
  interrupts();

  // 5. PRINT COUNT ONLY WHEN IT CHANGES
  if (countNow != previousCount)
  {
    Serial.printf("Products produced: %lu\n", (unsigned long)countNow);
    previousCount = countNow;
  }

  // 6. CALCULATE AND PRINT FREQUENCY ONLY WHEN IT CHANGES
  // A changed positive interval means a changed frequency
  if (timeNow > 0 && timeNow != previousTime)
  {
    float boxesPerSecond = 1000000.0f / timeNow;
    Serial.printf("Product output: %.2f per second\n", boxesPerSecond);
    previousTime = timeNow;
  }
}

