#include <Arduino.h>

#define SENSOR_PIN 21
#define WINDOW_MS 1000

volatile uint32_t count = 0;

uint32_t windowStartMs = 0;
uint32_t windowStartCount = 0;

uint32_t previousCount = 0;
float previousHz = 0;

//INTERRUPT: COUNT ONE PRODUCT
void IRAM_ATTR onProduct()
{
  count++;
}

void setup()
{
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);

  windowStartMs = millis();

  attachInterrupt(
    digitalPinToInterrupt(SENSOR_PIN),
    onProduct,
    RISING
  );
}

void loop()
{
  //COPY THE TIME AND COUNT
  noInterrupts();
  uint32_t now = millis();
  uint32_t countNow = count;
  interrupts();

  //PRINT TOTAL ONLY WHEN CHANGED
  if (countNow != previousCount)
  {
    Serial.printf(
      "Total products: %lu\n",
      (unsigned long)countNow
    );

    previousCount = countNow;
  }

  // CALCULATE RATE AFTER EACH WINDOW
  uint32_t elapsedMs = now - windowStartMs;

  if (elapsedMs >= WINDOW_MS)
  {
    uint32_t events = countNow - windowStartCount;

    float frequencyHz =
      events * 1000.0f / elapsedMs;

    // PRINT RATE ONLY WHEN CHANGED 
    if (frequencyHz != previousHz)
    {
      Serial.printf(
        "Product rate: %.3f per second\n",
        frequencyHz
      );

      previousHz = frequencyHz;
    }

    // ===== 11. START THE NEXT WINDOW =====
    windowStartMs = now;
    windowStartCount = countNow;
  }
}