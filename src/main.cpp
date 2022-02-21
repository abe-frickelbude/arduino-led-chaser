#include <Arduino.h>

const uint8_t NUM_LEDS = 15;

// Mapping of MCU pins to LED(0)...LED(n-1)
const uint8_t LED_OUTPUTS[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

// ADC input used for reading the delay time setting
const uint8_t ADC_PIN = A7;
const uint8_t ADC_PASSES = 100;

// values are in milliseconds
const uint16_t MIN_DELAY = 10;
const uint16_t MAX_DELAY = 1000;

//  ------------------ state ----------------------

uint8_t prevLedIndex = 0;
uint8_t currentLedIndex = 0;
uint16_t delayTime = 0;

// ------------------------------------------------

void setup()
{
  // initialize all LED pins as outputs and blank all LEDs
  for (uint8_t i = 0; i < NUM_LEDS; i++)
  {
    pinMode(LED_OUTPUTS[i], OUTPUT);
    digitalWrite(LED_OUTPUTS[i], LOW);
  }

  // delay time calculation in setup() instead of in loop() fixes the delay to a stable, de-noised value,
  // and also vastly improves performance by not repeating float calculations in the main loop,
  // but it can no longer be adjusted "interactively" (i.e you need to reset the MCU every time after adjusting)

  // read/accumulate
  float adcValue = 0;
  for (int i = 0; i < ADC_PASSES; i++)
  {
    adcValue += analogRead(ADC_PIN);
  }
  adcValue *= (1.0f / ADC_PASSES); // denoise
  adcValue *= max((1.0f / 1023), 1e-4); // normalize to [0..1] and clamp to .0001 to avoid numerical instabilities
  
  // linearly interpolate (but reversing the usual interpolation is (1-t)*A + t*B) 
  // to produce a final, linearly decreasing value from MAX_DELAY to MIN_DELAY as adcValue moves from [0...1]
  delayTime = (uint16_t)(round((adcValue * MIN_DELAY) + (1.0f - adcValue)*MAX_DELAY));
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
  delay(delayTime);
}
