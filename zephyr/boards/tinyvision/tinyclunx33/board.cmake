# Copyright (c) 2024 tinyVision.ai Inc.
# SPDX-License-Identifier: Apache-2.0

board_runner_args(ecpprog "")

include(${ZEPHYR_BASE}/boards/common/ecpprog.board.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../common/ecpprog_mpremote.board.cmake)

board_runner_args(openocd --cmd-pre-init "source [find tinyclunx33.cfg]")

include(${ZEPHYR_BASE}/boards/common/openocd.board.cmake)

# Run some code from RAM for performance during enumeration
zephyr_code_relocate(LIBRARY drivers__usb__udc LOCATION RAM)
#zephyr_code_relocate(LIBRARY subsys__usb__device_next LOCATION RAM) # Not enough RAM
