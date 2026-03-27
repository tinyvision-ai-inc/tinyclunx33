# Copyright (c) 2024 tinyVision.ai Inc.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory_ifdef(CONFIG_USB_DEVICE_STACK_NEXT usb/device_next)
add_subdirectory(usb/common)
