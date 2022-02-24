
#ifndef _VALUE_INTERPOLATOR_H
#define _VALUE_INTERPOLATOR_H

#include <avr/io.h> // for data types
#include <FixedPoints.h>
#include <FixedPointsCommon.h>

/*
 * A small utility class to allow for using ADC inputs as "dials" with user-definable min/max value ranges.
 */

class ValueInterpolator
{
private:
    // Warning: needs C++ 17 support to compile!
    static inline UFixed<16, 16> const NORM_FACTOR = 1023.0;
    static inline UFixed<16, 16> const MIN_NORMALIZED = 0.00001;
    static inline UFixed<16, 16> const MAX_NORMALIZED = 0.99999;

    uint16_t minValue; // minimum mapped value
    uint16_t maxValue; // maximum mapped value

    // interpolate the value in reverse
    // (i.e. as the input increases, the output will decrease from max to min)
    bool reverse;

public:
    ValueInterpolator(const uint16_t &minValue, const uint16_t &maxValue, const bool &reverse) : minValue(minValue),
                                                                                                 maxValue(maxValue),
                                                                                                 reverse(reverse)
    {
    }

    const uint16_t interpolate(const uint16_t &value)
    {
        UFixed<16, 16> normValue = value / NORM_FACTOR;

        // clamp to [0,1] if close to 0 or 1
        if(normValue < MIN_NORMALIZED) normValue = 0;
        if(normValue > MAX_NORMALIZED) normValue = 1;

        UFixed<16, 16> result;
        if (reverse)
        {
            result = maxValue - normValue * (maxValue - minValue);
        }
        else
        {
            result = minValue + normValue * (maxValue - minValue);
        }
        return roundFixed(result).getInteger();
    }
};

#endif