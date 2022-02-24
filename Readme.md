
# Arduino Nano LED chaser

A simple configurable LED chaser based on the Arduino Nano platform

## Features

* currently configured for 15 LEDs but can be adjusted by changing the `NUM_LEDS` constant and 
  reconfiguring the `LED_OUTPUTS` mapping array as necessary
* Basically limited by the number of I/O pins on the Nano minus the `PD0/PD1` and two other pins for ADC inputs
* VSCode PlatformIO configuration files are included
* All code is documented where possible

## Electrical notes

* The period (chase speed) is controlled by a 10K `[trimmer]` pot between +5V / GND connected to `A3`
* The duty cycle (flash duration) is controlled by a 10K `[trimmer]` pot between +5V / GND connected to `A4`
* A `0.1 uF` film capacitor can be connected between either pot wiper and ground to improve noise immunity
* A `~10-100 uF` electrolytic cap, along with a `0.1 uF` film cap can be connected across +5V/GND close to the
  Arduino board to improve stability

## Basic setup

* This is PlatformIO project, and will at least require the PlatformIO `cli` to build
* All external dependencies are declared in `platformio.ini`
* Requires C++ 17 to compile - any modern PlatformIO avr toolchain should be able to provide this assuming it's
  using GCC >= 7 
  * Be sure to add the corresponding compiler flag `-std=c++1z` to the `compilerArgs` section in  
  `.vscode/c_cpp_properties.json` (this is also present in `platformio.ini` but for some reason VSCode
  does not currently pick it up)

## Dependencies

* `FastAnalogRead` for smoothing / reducing jitter on ADC inputs (transitively depends on `FixedPoints`) 
* `FixedPoints` for improving performance by avoiding floating-point calculations in the `TimingCalculator` class
* `ArduinoLog` for comfortable logging of data to the serial port during development (all related code
  is conditionally included via the `LOG_VALUES_TO_SERIAL` symbol defined in `main.cpp`)

