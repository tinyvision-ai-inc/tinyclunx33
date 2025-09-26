USB CDC RAW Example
###################

This uses the CDC ACM USB class, but a custom ``usbd_cdc_raw.c`` implementation to allow
to enqueue data without using a ring buffer.

After programming the device and power cycling the board, a serial interface
would show-up, such as on ``/dev/ttyACM0`` on Linux, and everything written to it
will be read back after conversion to upper case:

.. code-block::

   picocom /dev/ttyACM0
