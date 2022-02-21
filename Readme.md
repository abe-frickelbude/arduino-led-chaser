
# Arduino Nano LED chaser

A simple LED chaser based on an Arduino Nano

* currently configured for 15 LEDs but can be adjusted by changing the `NUM_LEDS` constant and reconfiguring the `LED_OUTPUTS` 
  mapping array as necessary
* Basically limited by the number of I/O pins on the Nano minus the `PD0/PD1` and three other pins (two ADC inputs and one 
  switch/jumper)
* The chase delay time is controlled by a 10K `[trimmer]` pot between +5V / GND connected to `A7`
* The flash duration is controlled by a 10K `[trimmer]` pot between +5V / GND connected to `A6`
* A switch (or a jumper) from `19/A5` to GND can be used to switch between:
    * a "settings" mode for interactively picking the timing values with the pots (i.e. ADC inputs tied into the main loop) 
    * a "fixed" mode, once the pots have been set, to smooth and interpolate the inputs in `setup()` instead to provide 
      fixed, stable values and to avoid float calculations in the main loop.
* A `0.1 uF` film cap should be connected across the switch on pin 19 to provide some debouncing
* A `~10-100 uF` electrolytic cap, along with a `0.1 uF` film cap can be connected across +5V/GND close to the
  Arduino board to improve stability
* VSCode PlatformIO configuration files are included
* All code is documented where possible


