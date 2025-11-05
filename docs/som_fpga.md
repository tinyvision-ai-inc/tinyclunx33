# SoM FPGA {#som_fpga}

The tinyCLUNX33 SoM features the CrossLinkU-NX FPGA with 33k LUT, codename LIFCL-33U.

It features an integrated USB2 and USB3 MAC as well as an USB2 and USB3 PHY.

Programming the FPGA is done by writing the bitfile to the [Flash](som_flash.md)
that is attached to it. Upon power on, the FPGA will load it out of the first
512 kBytes, and operate the custom hardware described on that flash.


## Hardware integration

As seen in the [SoM Power](som_power.md) page, the I/O voltage levels of the
FPGA can be adapted through the PVDD pins, which are connected to the VCCIO
pins of the FPGA, controlling the banks voltage level.


## RTL integration

The [RTL Reference Design](rtl_reference_design.md) provides the foundation of
a system, on top of which the application can be implemented.

| Resource                 | Num or Size (total)   |
|--------------------------|-----------------------|
| Logic Cells              | 33k                   |
| 18*18 Multipliers        | 64                    |
| Embedded Block RAM (EBR) | 1152 kbit / 144 kByte |
| Distributed RAM          | 220 kbit / 27.5 kByte |
| Large RAM (LRAM)         | 2560 kbit / 320 kByte |
| 450 MHz Oscillator       | 1                     |
| 128 kHz Oscillator       | 1                     |
| GPLL                     | 1                     |
| USB2/USB3 MAC+PHY        | 1                     |


## Troubleshooting


### Q: I loaded a bad bitfile on my board and now the programmer refuses to program now!

Yes, this happens. Do not do it again! Here is the process to unlock your board:

1. Power off immediately, a bad bitfile may have bad pin allocations which can damage the SoM.
2. Short the `PROGn` pin on the `J14` header to Ground with a wire.
3. Power up the board. 
4. Check that the DONE LED is not lit up.
5. Your board should now be ready to get programmed.

### Q: Radiant Programmer complains that it could not read the IDCODE

This means it could at least reach the FTDI, but something went wrong for communicating with the FPGA.

You may need to verify which FTDI interface this was. Try changing from `FTUSB-1` to `FTUSB-0` for instance.


## Parts featured

- Lattice Semiconductor
  [LIFCL-33U-8CTG104CAS](https://www.latticesemi.com/Products/FPGAandCPLD/CrossLink-NX)
  FPGA (search for 33U in this page).
