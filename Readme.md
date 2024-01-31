## ch32v203-minimal

This repository is a companion to my ch32v003-minimal repository. The programs are similar. Some differences occur because:

1. The board exposes different GPIO pins. For instance, ch32v003 does not have GPIOB, whereas my ch32v203 has an onboard LED at pin PB2, which,
of course, is nice to use.

2. Each GPIO has 16 lines, instead of only 8 for ch32v003. For the extra lines there is an extra configuration register CFGHR.

3. The interrupt numbers are different. Refer to table 9-2 in the reference manual (not table 9-1, which is for ARM-based devices, this cost me
a lot of time).

4. ch32v203 has 64kB of flash, instead of only 16kB for ch32v003. For this reason it is easier to use the picolibc library.

5. ch32v003 has a 24MHz high-speed internal oscillator (HSI). Using the PLL, we can double this frequency to get a 48MHz clock. ch32v203 has a 8MHz internal
oscillator. However, the PLL can increase this frequency up to 18 times, giving a 144MHz clock. By default, the PLL uses half of the HSI frequency. To get full 144MHz
you need to configure the extended configuration register, documented in chapter 33 of the reference manual.

### Remarks

The following documentation has been used, in addition to the ones mentioned in the ch32v003-minimal repository:

[QingKeV4] The QingKeV4 Microprocessor Manual, can be obtained from <https://www.wch-ic.com/downloads/QingKeV4_Processor_Manual_PDF.html>.

[CH32v203] The CH32V203 Reference Manual, can be obtained from <https://www.wch-ic.com/downloads/CH32FV2x_V3xRM_PDF.html>.
