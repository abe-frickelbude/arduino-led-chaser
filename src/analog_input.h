
#ifndef _ANALOG_INPUT_H
#define _ANALOG_INPUT_H

#include <Arduino.h>

/*
 * A small utility function, along with associated parameter struct, to 
 * allow for using ADC inputs as "dials" with user-definable min/max value ranges. 
 * 
 */

// groups params for interpolateAdcInput() for better readability
struct AnalogInput
{
    uint16_t adcPin;   // ADC pin that the input is connected to, e.g. A3
    uint8_t numPasses; // number of ADC reads used for smoothing the input value
    uint16_t minValue; // minimum mapped value
    uint16_t maxValue; // maximum mapped value

    // interpolate the value in reverse
    // (i.e. as the ADC input increases, the output will decrease from max to min)
    bool reverse;
};

const uint16_t interpolateAdcInput(const AnalogInput &params)
{
    // read/accumulate
    float adcValue = 0;
    for (int i = 0; i < params.numPasses; i++)
    {
        adcValue += analogRead(params.adcPin);
    }
    adcValue *= (1.0f / params.numPasses); // denoise
    adcValue *= max((1.0f / 1023), 1e-4);  // normalize to [0..1] and clamp to .0001 to avoid numerical instabilities

    uint16_t result = 0;
    if (params.reverse)
    {
        // linearly interpolate (but reversing the usual interpolation formula (1-t)*A + t*B)
        // to produce a final, linearly decreasing value from maxValue to minValue as adcValue moves from [0...1]
        result = (uint16_t)(round(adcValue * params.minValue + (1.0f - adcValue) * params.maxValue));
    }
    else
    {
        // ditto, but with "normal" interpolation that produces an increasing value
        result = (uint16_t)(round((1.0f - adcValue) * params.minValue + adcValue * params.maxValue));
    }
    return result;
}

#endif