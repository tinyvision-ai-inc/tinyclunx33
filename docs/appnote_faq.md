# Frequently Asked Questions {#appnote_faq}

This app note covers various high level FAQ's about the capabilities of the platform as well as typical applications etc.

You may contact us via the
[chat server](https://discord.com/invite/3qbXujE) or by email is more than welcome.

## How to control the I2C for my sensor: Here are a couple of ways to do this:

1. Once the driver is properly written, the enumeration process will expose the exposure, gain etc. to the host UVC stack. UVC control changes now flow through to the I2C automagically.
2. Many of your sensor controls may not map directly to what UVC offers. In such a case, you can write your own protocol over a serial port. To do this:
  a. Use the exported Zephyr shell over the USB link and write your own commands there so that the application can talk to the Zephyr shell over a serial port.
  b. Export a dedicated serial port over the USB link and host your commands/applications over this link.

## What sensors can you support?

We can support any sensor (cameras, thermal, vibration, IMU, audio etc.) that has the following interfaces:

- 1/2/4 lane MIPI sensors with up to 1.2Gbps MIPI data rate
- LVDS, SPI, DVP sensor interfaces. IO pads on the FPGA are limited to 1.8V and an external translator is required to support 3.3V IO. 
- We bring out 1 bank of IO on the `Compute` module and 2 banks of IO on the `Connectivity` module.
  - Each bank must have the same voltage IO.
  - If you use MIPI, that bank voltage shall be set to 1.2V.
- A maximum of 27 pins on the `Compute` and 37 pins on the `Connectivity` module can be used to interface to the sensor, excluding the I2C.

## Unsupported sensor interfaces

- We use a soft MIPI which is limited to 1.2Gbps per link. Any data stream that requires a higher data rate than this cannot be supported. Suggest using a 4 Lane setup in this case.
- MIPI C-PHY: We do not support the new MIPI CPHY standard since the FPGA IO cells do not have this capability.

##  What is the maximum frame rate you can support?

The data path does not use the RISCV processor. As a result, there is no overhead for an increased frame rate. We have measured test patterns at well over 1.5kHz for smaller sizes. The bridge can sustain a data rate of 3.4Gbps for a single stream. The frame rate is not a consideration.

## How to allocate pins on the SoM for my own custom board?

The tinyCLUNX SoM brings out the FPGA banks 3 and/or 2 (Compute SoM only). Please follow these rules when allocating signals to your sensors on the SoM:

1. MIPI clock (or any interface with a clock) _shall_ be routed to the lanes with `PCLK` in them.
2. Interfaces with a single ended clock _shall_ be routed to `PCLK_P` pins.
3. The lanes within a _bank_ are length matched to each other. Differential signals such as MIPI that require length matching between lanes _shall_ use pairs within the same bank.
4. All signals within a single bank _shall_ use the same IO voltage.
