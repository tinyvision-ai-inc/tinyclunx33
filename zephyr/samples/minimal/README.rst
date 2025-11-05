Minimal Example
###############

This example is to be used in dire conditions where the CPU will refuse to boot Zephyr.

This disables all drivers except the essential ones, and the UART.
This acts as a "rescue" image to test that a system has most basic execution support on Zephyr.


Building
========

Visit the
`tinyvision_zephyr_sdk <https://github.com/tinyvision-ai-inc/tinyvision_zephyr_sdk>`_
documentation for a complete guide.

Example using the Rev2 SoM, Rev2 Devkit, FPGA releasea `rtl_1_0`:

.. code-block:: console

   west build --board tinyclunx33@rev2/rtl_1_0
   west flash


Running
=======

After programming the device and power cycling the board, use the serial console provided by the
FTDI such as ``/dev/ttyUSB1`` (2nd serial interface that shows-up) and observe the presence of
the boot banner:

.. code-block:: console

   $ picocom -q -b 90000 /dev/ttyUSB1
   *** Booting Zephyr OS build v4.0.0-3411-g589229cc5e12 ***

   uart:~$

Note that you might have to hit the reset switch of your board to catch the early boot logs.

Hitting ``<enter>`` is expected to make the shell prompt show-up.
