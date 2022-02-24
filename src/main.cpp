#include <Arduino.h>
#include <FastAnalogRead.h>
#include "ValueInterpolator.h"

// number of connected LEDs
const uint8_t NUM_LEDS = 15;

// Mapping of MCU pins to LED(0)...LED(n-1)
const uint8_t LED_OUTPUTS[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

/*
 * - The chase delay is controlled by 10K pot on A3
 * - The flash duration is controlled by 10K pot on A4
 * - min/max values are in milliseconds
 */

FastAnalogRead chaseDelayInput;
FastAnalogRead flashDurationInput;

ValueInterpolator chaseDelayInterpolator(1, 1000, true);
ValueInterpolator flashDurationInterpolator(1, 200, false);

//  ------------------ state ----------------------

uint8_t currentLedIndex = 0;
uint16_t chaseDelayTime = 50;
uint16_t flashDuration = 50;

// ------------------------------------------------

void setup()
{
  // Serial.begin(9600);

  // initialize all LED pins as outputs and blank all LEDs
  for (uint8_t i = 0; i < NUM_LEDS; i++)
  {
    pinMode(LED_OUTPUTS[i], OUTPUT);
    digitalWrite(LED_OUTPUTS[i], LOW);
  }

  // No "fast ADC" here as this produces cross-talk between inputs for some reason
  // as signal values increase
  chaseDelayInput.enableEdgeSnap();
  flashDurationInput.enableEdgeSnap();

  chaseDelayInput.begin(A3, true);
  flashDurationInput.begin(A4, true);
}

void loop()
{
  // read and interpolate control inputs
  flashDurationInput.update();
  chaseDelayInput.update();

  chaseDelayTime = chaseDelayInterpolator.interpolate(chaseDelayInput.getValue());
  flashDuration = flashDurationInterpolator.interpolate(flashDurationInput.getValue());

  // Serial.print("Flash time: ");
  // Serial.print(flashDuration, DEC);

  // Serial.print("\tChase time: ");
  // Serial.print(chaseDelayTime, DEC);

  // Serial.print("\tft: ");
  // Serial.print(ft, DEC);

  // Serial.print("\tcdt: ");
  // Serial.print(cdt, DEC);

  // Serial.print("\n");
  // delay(20);

  // --------- flash current LED -----------

  digitalWrite(LED_OUTPUTS[currentLedIndex], HIGH);
  delay(flashDuration);
  digitalWrite(LED_OUTPUTS[currentLedIndex], LOW);

  currentLedIndex++;
  if (currentLedIndex == NUM_LEDS)
  {
    currentLedIndex = 0;
  }

  // delay before switching to next LED
  delay(chaseDelayTime);
}
