# SoM JTAG {#som_jtag}

The tinyCLUNX33 FPGA has a JTAG interface attached, allowing to use the
[Lattice Radiant Reveal logic analyzer](https://www.latticesemi.com/~/media/328D471BF2C74EB1907832FAA6FB344B.ashx)
to inspect signals inside the FPGA as it runs.

Later in development of the tinyCLUNX33, it is also planned to make the soft
[VexRiscv](https://github.com/SpinalHDL/VexRiscv#embeddedRiscvJtag)
CPU core accessible through that same JTAG interface.
