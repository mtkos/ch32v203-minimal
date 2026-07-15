# the usb-hw program

The ch32v003 MCU does not have a USB peripheral. ch32v203 does, and it is almost identical to that of stm32f103. For that reason, the 3 programs using the USB peripheral are also very similar.
For explanation I refer to my stm32f103-minimal repository. There are only 2 significant differences:

1. ch32v203 has an internal pull-up resistor for the USB peripheral. It needs to be enabled through the extended configuration register (reference manual chapter 33).

2. There is some extra startup code, setting the gp (global pointer) register. In the preceding programs this was not necessary, but in general it is needed. The compiler expects it to be set,
and if not, some code optimisations do not work properly, causing the program to fail. The value for gp is defined in the linker script.
