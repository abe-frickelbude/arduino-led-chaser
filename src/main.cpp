#include <Arduino.h>

const uint8_t NUM_LEDS = 15;
const uint8_t LED_OUTPUTS[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

const uint8_t ADC_PIN = A7;
const uint8_t ADC_PASSES = 100;

const uint16_t MAX_DELAY = 1000; // milliseconds
const uint16_t MIN_DELAY = 990;  // milliseconds

// current led index
uint8_t prevLedIndex = 0;
uint8_t currentLedIndex = 0;
uint16_t chaseSpeed = 0;

void setup()
{
  // initialize all LED pins as outputs and blank all LEDs
  for (uint8_t i = 0; i < NUM_LEDS; i++)
  {
    pinMode(LED_OUTPUTS[i], OUTPUT);
    
    digitalWrite(LED_OUTPUTS[i], LOW);
  }

  // read/accumulate/normalize ADC input to [0..1]
  // accumulation in setup() instead of in loop() fixes the time delay to a stable, de-noised value,
  // but this can no longer adjusted "interactively" (i.e have to reset the MCU every time after adjusting)
  float adcValue = 0;
  for (int i = 0; i < ADC_PASSES; i++)
  {
    adcValue += analogRead(ADC_PIN);
  }
  adcValue *= (1.0f / ADC_PASSES);
  adcValue *= (1.0f / 1023);
}

void loop()
{
  digitalWrite(LED_OUTPUTS[prevLedIndex], LOW);
  digitalWrite(LED_OUTPUTS[currentLedIndex], HIGH);

  prevLedIndex = currentLedIndex;
  currentLedIndex++;

  if (currentLedIndex == NUM_LEDS)
  {
    prevLedIndex = NUM_LEDS - 1;
    currentLedIndex = 0;
  }

  // ------------------------------------

  uint16_t adcValue = analogRead(A7);
  uint16_t delayTime = MAX_DELAY - min(adcValue, MIN_DELAY);

  delay(delayTime);
}