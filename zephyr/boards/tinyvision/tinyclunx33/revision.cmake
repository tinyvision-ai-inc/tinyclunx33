# Copyright (c) 2024 tinyVision.ai Inc.
# SPDX-License-Identifier: Apache-2.0

set(VALID_BOARD_REVISIONS "rev1" "rev2" "rev3")

if(NOT BOARD_REVISION IN_LIST VALID_BOARD_REVISIONS)
    list(TRANSFORM VALID_BOARD_REVISIONS PREPEND "tinyclunx33@")
    list(JOIN VALID_BOARD_REVISIONS " or " VALID_BOARD_NAMES)
    message(FATAL_ERROR "Invalid revision \"${BOARD_REVISION}\", expecting: ${VALID_BOARD_NAMES}")
endif()

set(ACTIVE_BOARD_REVISION ${BOARD_REVISION})
