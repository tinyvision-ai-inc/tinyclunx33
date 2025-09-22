# Copyright (c) 2025 tinyVision.ai Inc.
# SPDX-License-Identifier: Apache-2.0

zephyr_library()

zephyr_library_sources_ifdef(CONFIG_FLASH_TINYVISION flash_tinyvision.c)
