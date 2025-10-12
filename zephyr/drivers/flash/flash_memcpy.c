/*
 * Copyright 2025 tinyVision.ai
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT zephyr_flash_memcpy

#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(flash_memcpy, CONFIG_FLASH_LOG_LEVEL);

#define FLASH_NODE DT_CHOSEN(zephyr_flash)

static const struct flash_parameters flash_memcpy_parameters = {
	.write_block_size = DT_PROP(FLASH_NODE, write_block_size),
	.erase_value = 0xff,
};

#if defined(CONFIG_FLASH_PAGE_LAYOUT)
static const struct flash_pages_layout flash_memcpy_pages_layout = {
	.pages_count = DT_REG_SIZE(FLASH_NODE) / DT_PROP(FLASH_NODE, erase_block_size),
	.pages_size = DT_PROP(FLASH_NODE, erase_block_size),
};

static void flash_memcpy_page_layout(const struct device *dev,
				    const struct flash_pages_layout **layout,
				    size_t *layout_size)
{
	*layout = &flash_memcpy_pages_layout;
	*layout_size = 1;
}
#endif

static bool flash_memcpy_access_is_valid(const struct device *dev, off_t offset, size_t len)
{
	/* Check for overflow */
	if (DT_REG_ADDR(FLASH_NODE) + offset + (off_t)len < offset) {
		return false;
	}

	if (offset < 0 || offset + len > DT_REG_SIZE(FLASH_NODE)) {
		return false;
	}

	return true;
}

static int flash_memcpy_read(const struct device *dev, off_t offset, void *data, size_t len)
{
	if (!flash_memcpy_access_is_valid(dev, offset, len)) {
		LOG_ERR("Invalid flash access from 0x%x to 0x%x",
			(size_t)offset, (size_t)(offset + len));
		return -EINVAL;
	}

	memcpy(data, (void *)(DT_REG_ADDR(FLASH_NODE) + offset), len);

	return 0;
}

static int flash_memcpy_write(const struct device *dev, off_t offset, const void *data, size_t len)
{
	if (!flash_memcpy_access_is_valid(dev, offset, len)) {
		return -EINVAL;
	}

	return -ENOTSUP;
}

static int flash_memcpy_erase(const struct device *dev, off_t offset, size_t len)
{
	if (!flash_memcpy_access_is_valid(dev, offset, len)) {
		return -EINVAL;
	}

	return -ENOTSUP;
}

static const struct flash_parameters *flash_memcpy_get_parameters(const struct device *dev)
{
	return &flash_memcpy_parameters;
}

static DEVICE_API(flash, flash_memcpy_driver_api) = {
	.read = flash_memcpy_read,
	.write = flash_memcpy_write,
	.erase = flash_memcpy_erase,
	.get_parameters = flash_memcpy_get_parameters,
#if defined(CONFIG_FLASH_HAS_PAGE_LAYOUT)
	.page_layout = flash_memcpy_page_layout,
#endif
};

static int _init(const struct device *dev) { printk("%s\n", __func__); return 0; }

DEVICE_DT_INST_DEFINE(0, _init, NULL, NULL, NULL, POST_KERNEL, CONFIG_FLASH_INIT_PRIORITY,
		      &flash_memcpy_driver_api);
