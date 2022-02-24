//#define LOG_VALUES_TO_SERIAL

#include <Arduino.h>
#include <FastAnalogRead.h>
#include "TimingCalculator.h"

#ifdef LOG_VALUES_TO_SERIAL
#include <ArduinoLog.h>
#endif

// number of connected LEDs
const uint8_t NUM_LEDS = 15;

// Mapping of MCU pins to LED(0)...LED(n-1)
const uint8_t LED_OUTPUTS[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

/*
 * - The period (1/chase speed) is controlled by 10K pot on A3
 * - The duty cycle (flash duration) is controlled by 10K pot on A4
 *
 *  The min / max period values are in milliseconds, duty cycle is expressed as a fraction of period, e.g. 0.3
 */
FastAnalogRead ledScanPeriodInput;
FastAnalogRead ledDutyCycleInput;
TimingCalculator timingCalculator(15, 500, 0.035, 0.50);

//  ################################# State ###########################################

// blink/scan timer
uint32_t startTime = 0L;
uint32_t currentTime = 0L;

uint8_t currentLedIndex = 0;
uint16_t ledScanPeriod = 1000;
uint16_t ledDutyCycle = 250;
bool ledActive = false; // optimization for avoiding redundant digitalWrite() calls

// ####################################################################################

void setup()
{
#ifdef LOG_VALUES_TO_SERIAL
  Serial.begin(9600);
  Log.begin(LOG_LEVEL_INFO, &Serial);
#endif

  // initialize all LED pins as outputs and blank all LEDs
  for (uint8_t i = 0; i < NUM_LEDS; i++)
  {
    pinMode(LED_OUTPUTS[i], OUTPUT);
    digitalWrite(LED_OUTPUTS[i], LOW);
  }

  // initialize ADC inputs on A3 and A4
  // Note: no "fast ADC" option here as this produces cross-talk between
  // inputs for some reason as signal values increase
  ledScanPeriodInput.begin(A3, true);
  ledDutyCycleInput.begin(A4, true);

  ledScanPeriodInput.enableEdgeSnap();
  ledDutyCycleInput.enableEdgeSnap();

  // sanity check - set initial control values from inputs
  ledDutyCycleInput.update();
  ledScanPeriodInput.update();
  timingCalculator.update(ledScanPeriodInput.getValue(), ledDutyCycleInput.getValue());
  ledScanPeriod = timingCalculator.getPeriod();
  ledDutyCycle = timingCalculator.getDutyCycle();

  // initialize timer
  startTime = millis();
}

void loop()
{
  // read ADC inputs
  ledDutyCycleInput.update();
  ledScanPeriodInput.update();

#ifdef LOG_VALUES_TO_SERIAL
  const auto period = ledScanPeriodInput.getValue();
  const auto dutyCycle = ledDutyCycleInput.getValue();
  timingCalculator.update(period, dutyCycle);

  Log.info(F("Period input: %d, d.c.input: %d, period: %d ms, t_on: %d ms" CR),
           period, dutyCycle, timingCalculator.getPeriod(), timingCalculator.getDutyCycle());
  delay(20);
#else

  currentTime = millis();
  if (currentTime - startTime < ledDutyCycle && !ledActive)
  {
    // turn on current LED
    digitalWrite(LED_OUTPUTS[currentLedIndex], HIGH);
    ledActive = true;
  }

  if (currentTime - startTime >= ledDutyCycle && ledActive)
  {
    // t_on exceeded, turn off current LED
    digitalWrite(LED_OUTPUTS[currentLedIndex], LOW);
    ledActive = false;
  }

  // advance to next LED
  if (currentTime - startTime >= ledScanPeriod)
  {
    currentLedIndex++;
    if (currentLedIndex == NUM_LEDS)
      currentLedIndex = 0;   // cycle back to first LED
    startTime = currentTime; // reset timer
  }

  // update inputs, but save some performance - don't run calculations if inputs didn't change
  if (ledScanPeriodInput.hasChanged() || ledDutyCycleInput.hasChanged())
  {
    timingCalculator.update(ledScanPeriodInput.getValue(), ledDutyCycleInput.getValue());
    ledScanPeriod = timingCalculator.getPeriod();
    ledDutyCycle = timingCalculator.getDutyCycle();
  }

#endif
}
