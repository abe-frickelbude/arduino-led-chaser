#include <Arduino.h>
#include "analog_input.h"

// number of connected LEDs
const uint8_t NUM_LEDS = 15;

// Mapping of MCU pins to LED(0)...LED(n-1)
const uint8_t LED_OUTPUTS[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

/*
 * - The flash duration is controlled by 10K pot on A6 
 * - The chase delay is controlled by 10K pot on A7
 * - min/max values are in milliseconds
 */
const AnalogInput CHASE_DELAY_INPUT = {
  .adcPin = A7, .numPasses = 100, .minValue = 1, .maxValue = 1500, .reverse = true
};

const AnalogInput FLASH_TIME_INPUT = {
   .adcPin = A6, .numPasses = 100, .minValue = 1, .maxValue = 250, .reverse = false
};

//  ------------------ state ----------------------

uint8_t currentLedIndex = 0;
uint16_t chaseDelayTime = 0;
uint16_t flashTime = 50;

// ------------------------------------------------

void setup()
{
  // initialize all LED pins as outputs and blank all LEDs
  for (uint8_t i = 0; i < NUM_LEDS; i++)
  {
    pinMode(LED_OUTPUTS[i], OUTPUT);
    digitalWrite(LED_OUTPUTS[i], LOW);
  }

  // TODO: add interpolation here based on mode switch on pin 19
  // delay time calculation in setup() instead of in loop() fixes the delay to a stable, de-noised value,
  // and also vastly improves performance by not repeating float calculations in the main loop,
  // but it can no longer be adjusted "interactively" (i.e you need to reset the MCU every time after adjusting)
}

void loop()
{

  // read and interpolate control inputs
  flashTime = interpolateAdcInput(FLASH_TIME_INPUT);
  chaseDelayTime = interpolateAdcInput(CHASE_DELAY_INPUT);

  // --------- flash current LED -----------

  digitalWrite(LED_OUTPUTS[currentLedIndex], HIGH);
  delay(flashTime);
  digitalWrite(LED_OUTPUTS[currentLedIndex], LOW);

  currentLedIndex++;
  if (currentLedIndex == NUM_LEDS)
  {
    currentLedIndex = 0;
  }

  // delay before switching to next LED
  delay(chaseDelayTime);
}
