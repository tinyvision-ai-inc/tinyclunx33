# Firmware Intro {#appnote_firmware}

On top of the CrossLinkU-NX (`LIFCL-33U`) FPGA chip provided by
Lattice Semiconductor, tinyVision.ai provides:

* An **SoC system** is running on it, is using a
  [VexRiscv](https://github.com/SpinalHDL/VexRiscv/)
  RISC-V core along with a [LiteX](https://github.com/enjoy-digital/litex)
  allowing it to execute an RTOS (timers, UART, SPI flash...).

* Then **extra peripherals** are integrated such as the USB23 core, a
  I2C controller, and application-specific video processing elements,
  which can be controlled from the CPU.

* **Zephyr integration** through a
  [`tinyclunx33`](https://github.com/tinyvision-ai-inc/tinyclunx33_sdk)
  providing a working base system featuring an USB stack, USB23 driver,
  an USB Video Class and everything needed to provide end-to-end
  connectivity with host application over USB3 such as a video feed.

* An optional driver called **uvcmanager** permitting to reach 3.4 Gbit/s
  transfer speed end-to-end at the application level.

Being completely compatible with Zephyr, firmware for the tinyCLUNX33
can be built given extra configuration files and drivers, using the
regular build process of Zephyr.

## Firmware Execution

The firmware is run by the system loaded into the FPGA.
First, there need to be an FGPA system image
[loaded into the tinyCLUNX33 flash](https://tinyclunx33.tinyvision.ai/som_flash.html).

This will contain the definition of the RISC-V CPU system described above,
and the firmware will be able to start from address 0x00100000 of the flash.
On Linux systems, this second part is handled by the `west flash` command.

## Firwmare Debugging

![Devkit debug USB port and FTDI chip](tinyclunx33_usb_to_mipi_devkit_debug.png)

The FTDI chip present on the Devkit has the following roles:

- Programming the FPGA system and firmware images in flash.

- UART access for debug logs and interactive
  [Zephyr shell](https://docs.zephyrproject.org/latest/services/shell/index.html).

- JTAG access (work in progresss)

The Zephyr system offers configurable log levels, which can be turned up and down
from the `prj.conf` file of each application.

Future versions of the SoC may integrate JTAG for GDB-based debugging.
