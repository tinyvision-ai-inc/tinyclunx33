# Copyright (c) 2025 Silicon Laboratories Inc.
# SPDX-License-Identifier: Apache-2.0

zephyr_library()

# zephyr-keep-sorted-start
zephyr_library_sources_ifdef(CONFIG_DEBUG_SPINEX_EVENT_LOGGER spinex-event-logger.c)
zephyr_library_sources_ifdef(CONFIG_DEBUG_SPINEX_REG_FILE spinex-reg-file.c)
# zephyr-keep-sorted-stop
