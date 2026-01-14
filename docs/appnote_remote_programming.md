# Remote programming of the tinyCLUNX33 {#appnote_remote_programming}

This appnote explains how it is possible to manage a tinyCLUNX33-based device from remote,
permitting to automate some testing or give access to ditant developers to a local device.

Devices targeted are development kits and custom devices.

This assumes a linux-based host computer constantly available via SSH, such as via port forwarding
or via a VPN. Advise to disable password-based log-in for security reasons, and only use SSH keys.

The host name this sample taken as example is `user@192.168.100.100`, and to be replaced by the
user and host name of your remote machine.


## Flashing a firmware and bitfile from remote

The [`ecpprog`](https://github.com/gregdavill/ecpprog) program can be used to flash a binary at
a specified address of the flash. This uses an FTDI adapter connected to the tinyCLUNX33, such as
the one provided on the DEBUG interface of the tinyCLUNX33 devkit.

To flash a local binary to a remote devkit across an:

```
ssh user@192.168.100.100 ecpprog -o 0x000000 - <path/to/bitfile.bit
```

It is possible to pass extra flags to `ecpprog` to specify a particular device when multiple are
present: For instance, assuming `lsusb` shows a lines like this:

```
Bus 003 Device 038: ID 0403:6014 Future Technology Devices International, Ltd FT232H Single HS USB-UART/FIFO IC
```

The `ecpprog` command would be like this:

```
ssh user@192.168.100.100 ecpprog -o 0x000000 -d d:003/003 - <path/to/bitfile.bit
```

It is possible to replace `0x000000` by a different address, for instance to flash a firmware.

If this fails, it is possible that the default speed is too slow, and it is possible to select
the divider with the `-k` flag:

```
ssh user@192.168.100.100 ecpprog -o 0x000000 -d d:003/003 -k 10 - <path/to/bitfile.bit
```


## Remote power cycle: StarTech.com managed USB HUBs

The StarTech.com managed hubs such as
[this one](https://www.startech.com/en-us/usb-hubs/5g4aindrm-usb-a-hub) use a command line utility
with a name like `cusbi`, provided as a binary.

They are also compatible with the standard `uhubctl` command:

```
# uhubctl
```
```
Current status for hub 4-1 [14b0:0438 StarTech.com USB3.0 4 Port HUB 000000000, USB 3.20, 4 ports, ppps]
  Port 1: 02a0 power 5gbps Rx.Detect
  Port 2: 02a0 power 5gbps Rx.Detect
  Port 3: 02a0 power 5gbps Rx.Detect
  Port 4: 02a0 power 5gbps Rx.Detect
Current status for hub 3-1 [14b0:0439 StarTech.com USB2.0 4 Port HUB 000000000, USB 2.10, 4 ports, ppps]
  Port 1: 0100 power
  Port 2: 0100 power
  Port 3: 0503 power highspeed enable connect [0403:6014 tinyVision.ai UPduino v3.1]
  Port 4: 0103 power enable connect [0403:6001 StarTech.com 5G4AINDRM-USB-A-HUB B0AOGN53]
```

Then to select only `hub 3-1` and only `port 3`, and perform a power cycle:

```
# uhubctl --location 3-1 --port 3 --action cycle
```
```
Current status for hub 4-1 [14b0:0438 StarTech.com USB3.0 4 Port HUB 000000000, USB 3.20, 4 ports, ppps]
  Port 3: 02a0 power 5gbps Rx.Detect
Sent power off request
New status for hub 4-1 [14b0:0438 StarTech.com USB3.0 4 Port HUB 000000000, USB 3.20, 4 ports, ppps]
  Port 3: 00a0 off
Current status for hub 3-1 [14b0:0439 StarTech.com USB2.0 4 Port HUB 000000000, USB 2.10, 4 ports, ppps]
  Port 3: 0503 power highspeed enable connect [0403:6014 tinyVision.ai UPduino v3.1]
Sent power off request
New status for hub 3-1 [14b0:0439 StarTech.com USB2.0 4 Port HUB 000000000, USB 2.10, 4 ports, ppps]
  Port 3: 0000 off
Current status for hub 4-1 [14b0:0438 StarTech.com USB3.0 4 Port HUB 000000000, USB 3.20, 4 ports, ppps]
  Port 3: 00a0 off
Sent power on request
New status for hub 4-1 [14b0:0438 StarTech.com USB3.0 4 Port HUB 000000000, USB 3.20, 4 ports, ppps]
  Port 3: 02a0 power 5gbps Rx.Detect
Current status for hub 3-1 [14b0:0439 StarTech.com USB2.0 4 Port HUB 000000000, USB 2.10, 4 ports, ppps]
  Port 3: 0000 off
Sent power on request
New status for hub 3-1 [14b0:0439 StarTech.com USB2.0 4 Port HUB 000000000, USB 2.10, 4 ports, ppps]
  Port 3: 0101 power connect [0403:6014]
```

Then `dmesg` confirms us that the device was power cycled for hub 3-1 port 3:

```
# dmesg
```
```
[4053235.823576] usb 3-1.3: new high-speed USB device number 44 using xhci_hcd
[4053235.964218] usb 3-1.3: New USB device found, idVendor=0403, idProduct=6014, bcdDevice= 9.00
[4053235.964234] usb 3-1.3: New USB device strings: Mfr=1, Product=2, SerialNumber=0
[4053235.964243] usb 3-1.3: Product: UPduino v3.1
[4053235.964249] usb 3-1.3: Manufacturer: tinyVision.ai
[4053235.969087] ftdi_sio 3-1.3:1.0: FTDI USB Serial Device converter detected
[4053235.969196] usb 3-1.3: Detected FT232H
[4053235.970054] usb 3-1.3: FTDI USB Serial Device converter now attached to ttyUSB0
```


## Remote power cycle: any MicroPython-based devboard

Basic usage:

```
west flash --runner ecpprog_mpremote --user=user --host=192.168.100.100 \
  --mpremote=remote/path/to/mpremote --ecpprog=remote/path/to/ecpprog
```

See the Zephyr [`ecpprog_mpremote`](https://github.com/tinyvision-ai-inc/tinyclunx33/blob/main/zephyr/scripts/ecpprog_mpremote.py)
source for a complete

[`mpremote`](https://pypi.org/project/mpremote/) and
[`ecpprog`](https://github.com/gregdavill/ecpprog) needs to be installed on the remote host.


## GPIO control with the FTDI

The FTDI chips typically used with various tinyCLUNX33 devices (such as the devkit
or using the UPduino JTAG as adapter) are compatible with the
[`pyftdi`](https://eblot.github.io/pyftdi/gpio.html) python library.

From the `lsusb` output, identify which FTDI chip type is used (such as FT232H or FT2232H, etc)
then extract the lowercase part number after the `FT` prefix, such as `2232h`, for use in
the script below:

```python
from pyftdi.gpio import GpioAsyncController
import time

PIN_NUM = 5
STATE = 0  # 0 for Low, 1 for High

gpio = GpioAsyncController()
gpio.configure('ftdi://:2232h/1', direction=(1 << PIN_NUM))
gpio.write(STATE << PIN_NUM)
gpio.close()
```

Upon running the script, this will configure the GPIO in the selected state.

This can be used for power cycling the board, or controlling the GPIO state.

UART and GPIO cannot be used simultaneously: once `gpio.close()` is called,
the USB FTDI interface is switched away from GPIO (pins are reset to their
default value) and allows the UART to be accessed again.
