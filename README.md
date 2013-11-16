# Updating keyboard layout

* Run teensy.app from teensy.dmg
* Press +L1 then teensy button
* Open newest .hex file
* Program
* Reboot

Electronics

The controller is based on a Teensy 2.0 AVR microcontroller board in the right half, with a I/O expander in the left half. The halves are connected with a 4-lead cable with power and i2c lines to the I/O expander. A mini-USB port is on the right PCB. The controller board is connected to the PCB with both pins and an internal USB cable.

The PCB design is reversible: the same type of PCB is used for both the left and right side. This means that it is possible to reverse the positions of the Teensy and the I/O expander, but then they would have to be mounted upside-down. Jumpers on the top of the PCB are also used.

The Cherry MX switches can be PCB-mounted, or be plate-mounted if a mounting plate is used (which they are in the official case designs). There is a diode for each switch, that can be either surface-mounted, through-hole, or through-hole, mounted inside most switches.

The diodes are supposed to be facing the other way on the left keyboard half, but the direction could also be swapped in firmware by flipping the values of "DRIVE_ROWS" and "DRIVE_COLUMNS" in "options.h".[1]

Component list below (adapted from [2]):

* 2	ErgoDox PCB
* 1	Teensy USB Board, Version 2.0
* 1	MCP23018-E/SP I/O expander
* 76-80	Cherry MX switches, PCB or plate-mounted
* 76-80	1N4148 diodes, SOD-123 package or DO-35 (0.3" pitch)
* 2	2.2 kΩ resistors (red, red, red)
* 3	3mm T1 LEDs
* 3	~220 Ω resistors, or match to LED. (red, red, brown)
* 5	Short jumpers
* 1	0.1 µF ceramic capacitor (marked "104" for 10*104 picofarad). Not strictly necessary
* 1	USB mini B connector WM17115.¹
* 1	USB mini B plug with short cable (such as H2955).¹
* 1	USB cable male A to male mini B.¹
* 2	3.5 mm TRRS sockets, CP-43514 but FC68129 may also work with extra pins snipped off.²
* 1	Cable with two 3.5 mm TRRS plugs.²

Notes:

1. The USB cable could also be routed directly to the Teensy 2.0, but then an additional wire has to be connected from Teensy pin VCC to the pad marked "5V" near the plug's position. GND should also be connected.

2. A fixed cable could also be used instead of TRRS cable and plugs.

Backlit version

The backlit PCB uses a single "high power" SMD mounted RGB LED on each half connected to high efficiency LED drivers controlled by PWM and I2C bus.

On top of the LED there is a light pipe which directs the light into .75mm fiber optic strands, these are then individually routed on the back of the PCB to each switch where they are fixed using the standard LED hole in the PCB/Switch.

This gives the ability for the back-light on each half to be dynamically set to any RGB value.

The development is hosted on GitHub at: [2]

The new back-light enabled PCB fixes the outermost key columns bug and is fully backwards compatible, it will function as the original when the back-light components are absent.

The following table lists the additional components required for the back light:

Existing cases can be modified for the backlit PCB by removing material to make room for the light tube.

* 2	LM3549 Sequential RGB LED drivers
* 2	LPF-R011362S 90° Light tube, square end.
* 2	LRTB C9TP RGB LEDs 30-250mA per color
* 76-80	0.75mm Fiber Optic cables, 200mm(8")
* 2	C1206C106J8RACTU 1206 Footprint 10 µF Capacitor ,6.3V/10V (X7R/X8R)
* 2	C1206C475J8NACTU 1206 Footprint 4.7 µF Capcitor ,6.3V/10V (X7R/X8R)
* 2	L1210R2R2MDWIT 1210 Footprint 2.2 µH Inductor, 1900/2000 mA

Total additional cost is expected to be between 15-20 dollars for both halves depending on volume.
