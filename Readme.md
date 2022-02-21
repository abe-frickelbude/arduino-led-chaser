
# Arduino Nano LED chaser


A simple LED chaser based on an Arduino Nano

* currently configured for 15 LEDs but can be adjusted by changing the `NUM_LEDS` constant and reconfiguring the `LED_OUTPUTS` mapping array as necessary
* Basically limited by the number of I/O pins on the Nano minus the `PD0/PD1`, along with a single ADC input used for setting the delay time
* Time delay value is accumulated and de-noised in `setup()` to provide a fixed, stable value and to avoid float calculations in the main loop
* VSCode PlatformIO configuration files are included
* All code is in single main.cpp and is well-documented where possible


