USB Shell Example
#################

Building
========

Visit the
`tinyvision_zephyr_sdk <https://github.com/tinyvision-ai-inc/tinyvision_zephyr_sdk>`_
documentation for a complete guide.

Example using the Rev2 SoM, Rev2 Devkit, FPGA release `rtl_1_0`:

.. code-block:: console

   west build --board tinyclunx33@rev2/rtl_1_0
   west flash


Running
=======

After programming the device and power cycling the board, a serial interface would show-up,
such as ``/dev/ttyACM0`` or ``/dev/ttyACM1`` on Linux, or ``COM0``, ``COM1`` on Windows.

You can then connect to it using a serial console viewer such as picocom or puTTY.

.. code-block:: console

   picocom /dev/ttyACM0

The baudrate is ignored as this is a virtual serial port over USB.
