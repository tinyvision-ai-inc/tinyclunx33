# Copyright (c) 2024 tinyVision.ai Inc.
# SPDX-License-Identifier: Apache-2.0

include(${ZEPHYR_BASE}/boards/common/ecpprog.board.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../common/ecpprog_mpremote.board.cmake)

board_runner_args(openocd --cmd-pre-init "source [find tinyclunx33.cfg]")

include(${ZEPHYR_BASE}/boards/common/openocd.board.cmake)
