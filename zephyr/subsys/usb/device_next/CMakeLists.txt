# Copyright (c) 2024 tinyVision.ai Inc.
# SPDX-License-Identifier: Apache-2.0

zephyr_library()

zephyr_library_sources_ifdef(CONFIG_USBD_CDC_RAW_CLASS class/usbd_cdc_raw.c)
zephyr_library_sources_ifdef(CONFIG_USBD_CDC_MUTE_CLASS class/usbd_cdc_mute.c)
