USB CDC Mute Example
####################

This uses the CDC ACM USB class, but a custom ``usbd_cdc_mute.c`` implementation to allow
to enumerate as CDC ACM without any I/O done by software, which is convenient to access
the USB endpoints directly from RTL.

After programming the device and power cycling the board, a serial interface
would show-up, such as on ``/dev/ttyACM0`` on Linux, and what I/O happens over this
interface is under the custom RTL's control.

.. code-block::

   picocom /dev/ttyACM0

If interleaving custom RTL and software, the endpoint numbers need to match, and this is
done by observing the endpoint numbers after the firmware is built. For instance, using
the ``lsusb -v -d 1209:0001`` command.
