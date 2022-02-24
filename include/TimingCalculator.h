
#ifndef TIMING_CALCULATOR_H
#define TIMING_CALCULATOR_H

#include <avr/io.h>
#include <FixedPoints.h>
#include <FixedPointsCommon.h>

class TimingCalculator
{

private:
    // Warning: needs C++ 17 support to compile!
    static inline UQ16x16 const NORM_FACTOR = 1023.0; // normalization factor for ADC inputs
    static inline UQ16x16 const MIN_NORMALIZED = 0.00001;
    static inline UQ16x16 const MAX_NORMALIZED = 0.99999;

    uint16_t const _minPeriod;
    uint16_t const _maxPeriod;

    UQ16x16 const _minDutyCycle;
    UQ16x16 const _maxDutyCycle;

    // state
    uint16_t _period;    // T = t_off + t_on
    uint16_t _dutyCycle; // t_on

public:
    TimingCalculator(const uint16_t &minPeriod, const uint16_t &maxPeriod,
                     const UQ16x16 minDutyCycle = 0.05, const UQ16x16 maxDutyCycle = 0.95) : _minPeriod(minPeriod),
                                                                                             _maxPeriod(maxPeriod),
                                                                                             _minDutyCycle(minDutyCycle),
                                                                                             _maxDutyCycle(maxDutyCycle),
                                                                                             _period(0),
                                                                                             _dutyCycle(0)
    {
    }

    inline uint16_t getPeriod() const { return _period; }
    inline uint16_t getDutyCycle() const { return _dutyCycle; }

    /**
     * Update and recalculate period and tOn (duty cycle) from two input values taken from
     * ADC inputs in the range [0,1023] (assuming 10-bit ADC resolution)
     *
     * @param period
     * @param dutyCycle
     */
    void update(const uint16_t &period, const uint16_t &dutyCycle)
    {

        UQ16x16 normValue = normalizeValue(period, MIN_NORMALIZED, MAX_NORMALIZED);
        UQ16x16 mappedPeriod = _minPeriod + normValue * (_maxPeriod - _minPeriod);

        normValue = normalizeValue(dutyCycle, _minDutyCycle, _maxDutyCycle);
        UQ16x16 mappedDutyCycle = normValue * mappedPeriod;

        _period = roundFixed(mappedPeriod).getInteger();
        _dutyCycle = roundFixed(mappedDutyCycle).getInteger();
    }

private:
    UQ16x16 normalizeValue(const uint16_t &value, const UQ16x16 &min, const UQ16x16 &max)
    {
        UQ16x16 normValue = value / NORM_FACTOR;
        // clamp to [min,max]
        if (normValue < min)
            normValue = min;
        if (normValue > max)
            normValue = max;
        return normValue;
    }
};

#endif