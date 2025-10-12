/*
 * Copyright 2025 tinyVision.ai
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#define DT_DRV_COMPAT zephyr_flash_memcpy

#define LOG_LEVEL CONFIG_FLASH_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(flash_memcpy);

#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>

static const struct flash_parameters flash_memcpy_parameters = {
	.write_block_size = 1,
	.erase_value = 0xff,
};

static int flash_memcpy_read(const struct device *dev, off_t offset,
			   void *data, size_t len)
{
	memcpy(data, (void *)offset, len);
	return 0;
}

static int flash_memcpy_write(const struct device *dev, off_t offset,
			    const void *data, size_t len)
{
	return -ENOTSUP;
}

static const struct flash_parameters*
flash_memcpy_get_parameters(const struct device *dev)
{
	ARG_UNUSED(dev);

	return &flash_memcpy_parameters;
}

static DEVICE_API(flash, flash_memcpy_driver_api) = {
	.read = flash_memcpy_read,
	.write = flash_memcpy_write,
	.get_parameters = flash_memcpy_get_parameters,
};

static int flash_memcpy_init(const struct device *dev)
{
	return 0;
}

DEVICE_DT_INST_DEFINE(0, flash_memcpy_init, NULL,
		      NULL, NULL, POST_KERNEL,
		      CONFIG_FLASH_INIT_PRIORITY, &flash_memcpy_driver_api);
