# the Reflection Chamber
a light-based kinetic automaton made with Arduino and Neopixel.

![thumbnail](https://user-images.githubusercontent.com/34627009/102437159-11e33e80-3fe8-11eb-935c-fd64bbaaa15e.png)

demo video for ver 0.1.0 is available at https://www.youtube.com/watch?v=thk6kFnlshk

---

## code structure
the software is split into two separate Arduino programs, both running on their separate microcontroller unit and communicating via I2C:
1. `trc_motor`: manages stepper motor actualization and pressure sensor assignation. is also responsible for motor sequencing;
2. `trc_lights`: manages everything relating to light, lasers, cells and NeoPixels.

since this installation requires a substantial amount of pins, the PCB responsible for data transmission between Arduino and the components implements a lot of shift registers for digital/binary data (motors and lasers) and de/multiplexers for analog/PWM data (photocells, pressure sensors, parasite lights).
