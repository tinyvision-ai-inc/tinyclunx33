/*
 * Copyright 2025 tinyVision.ai
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#define DT_DRV_COMPAT tinyvision_flash

#define LOG_LEVEL CONFIG_FLASH_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(flash_tinyvision);

#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>

#define SPI_TINYVISION_DATA        0x00
#define SPI_TINYVISION_BUFFER      0x04
#define SPI_TINYVISION_CONFIG      0x08
#define SPI_TINYVISION_INTERRUPT   0x0C
#define SPI_TINYVISION_CLK_DIVIDER 0x20
#define SPI_TINYVISION_SS_SETUP    0x24
#define SPI_TINYVISION_SS_HOLD     0x28
#define SPI_TINYVISION_SS_DISABLE  0x2C

#define SPI_TINYVISION_CMD_WRITE (1 << 8)
#define SPI_TINYVISION_CMD_READ (1 << 9)
#define SPI_TINYVISION_CMD_SS (1 << 11)

#define SPI_TINYVISION_RSP_VALID (1 << 31)

#define SPI_TINYVISION_STATUS_CMD_INT_ENABLE (1 << 0)
#define SPI_TINYVISION_STATUS_RSP_INT_ENABLE (1 << 1)
#define SPI_TINYVISION_STATUS_CMD_INT_FLAG (1 << 8)
#define SPI_TINYVISION_STATUS_RSP_INT_FLAG (1 << 9)


#define SPI_TINYVISION_MODE_CPOL (1 << 0)
#define SPI_TINYVISION_MODE_CPHA (1 << 1)


/** Working data for the device */
struct spi_tinyvision_config {
	uint32_t reg;
	uint32_t cpol;
	uint32_t cpha;
	uint32_t mode;
	uint32_t clkDivider;
	uint32_t ssSetup;
	uint32_t ssHold;
	uint32_t ssDisable;
};

static inline uint32_t read_u32(uint32_t address){
    return *((volatile uint32_t*) address);
}

static inline void write_u32(uint32_t data, uint32_t address){
    *((volatile uint32_t*) address) = data;
}

static inline uint16_t read_u16(uint32_t address){
    return *((volatile uint16_t*) address);
}

static inline void write_u16(uint16_t data, uint32_t address){
    *((volatile uint16_t*) address) = data;
}

static inline uint8_t read_u8(uint32_t address){
    return *((volatile uint8_t*) address);
}

static inline void write_u8(uint8_t data, uint32_t address){
    *((volatile uint8_t*) address) = data;
}

static inline void write_u32_ad(uint32_t address, uint32_t data){
    *((volatile uint32_t*) address) = data;
}

static uint32_t spi_tinyvision_cmdAvailability(uint32_t reg)
{
    return read_u32(reg + SPI_TINYVISION_BUFFER) & 0xFFFF;
}
static uint32_t spi_tinyvision_rspOccupancy(uint32_t reg)
{
    return read_u32(reg + SPI_TINYVISION_BUFFER) >> 16;
}

/*static uint8_t spi_tinyvision_write_read(uint32_t reg, uint8_t data)
{
    while(spi_tinyvision_cmdAvailability(reg) == 0);
    write_u32(data | SPI_TINYVISION_CMD_READ | SPI_TINYVISION_CMD_WRITE, reg + SPI_TINYVISION_DATA);
    while(spi_tinyvision_rspOccupancy(reg) == 0);
    return read_u32(reg + SPI_TINYVISION_DATA);
}*/

static uint8_t spi_tinyvision_read(uint32_t reg)
{
    while(spi_tinyvision_cmdAvailability(reg) == 0);
    write_u32(SPI_TINYVISION_CMD_READ, reg + SPI_TINYVISION_DATA);
    while(spi_tinyvision_rspOccupancy(reg) == 0);
    return read_u32(reg + SPI_TINYVISION_DATA);
}

static void spi_tinyvision_write(uint32_t reg, uint8_t data)
{
    while(spi_tinyvision_cmdAvailability(reg) == 0);
    write_u32(data | SPI_TINYVISION_CMD_WRITE, reg + SPI_TINYVISION_DATA);
}

static void spi_tinyvision_select(uint32_t reg, uint32_t slaveId)
{
    while(spi_tinyvision_cmdAvailability(reg) == 0);
    write_u32(slaveId | 0x80 | SPI_TINYVISION_CMD_SS, reg + SPI_TINYVISION_DATA);
}

static void spi_tinyvision_deselect(uint32_t reg, uint32_t slaveId)
{
    while(spi_tinyvision_cmdAvailability(reg) == 0);
    write_u32(slaveId | 0x00 | SPI_TINYVISION_CMD_SS, reg + SPI_TINYVISION_DATA);
}

#define SOC_NV_FLASH_ADDR 0x20000000
#define DELAY_SETTLE 2000
#define DELAY_SETTLE_SHORT 30

#define WRITE_SIZE DT_PROP(DT_CHOSEN(zephyr_flash), write_block_size)
#define ERASE_SIZE DT_PROP(DT_CHOSEN(zephyr_flash), erase_block_size)
#define TOTAL_SIZE DT_REG_SIZE(DT_CHOSEN(zephyr_flash))

struct flash_tinyvision_config {
	uint32_t reg;
};

static struct flash_tinyvision_config flash_config = {
	.reg = DT_INST_REG_ADDR(0),
};

static const struct flash_parameters flash_tinyvision_parameters = {
	.write_block_size = WRITE_SIZE,
	.erase_value = 0xff,
};

static bool flash_tinyvision_valid_range(off_t offset, size_t size)
{
	if (offset > TOTAL_SIZE) return false;
	if (size + offset > TOTAL_SIZE) return false;
	return true;
}

static int flash_tinyvision_read(const struct device *dev, off_t offset,
			   void *data, size_t len)
{
	if (len == 0U) {
		return 0;
	}

	if (!flash_tinyvision_valid_range(offset, len)) {
		return -EINVAL;
	}

	memcpy(data, (uint8_t *)SOC_NV_FLASH_ADDR + offset, len);

	return 0;
}

static int flash_tinyvision_write(const struct device *dev, off_t offset,
			    const void *data, size_t len)
{
	const struct spi_tinyvision_config *cfg = dev->config;
	int ret = 0;

	if (len == 0U) {
		return 0;
	}

	if (!flash_tinyvision_valid_range(offset, len)) {
		return -EINVAL;
	}

	uint32_t lock_key = irq_lock();

	/* clear icache */
	__asm__ volatile(
		".insn 0x100F\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
	);

	volatile int time = DELAY_SETTLE;
	while (time > 0) {
		time--;
	}

	for (size_t i = 0; i < len; i += WRITE_SIZE) {
		spi_tinyvision_select(cfg->reg, 0);
		spi_tinyvision_write(cfg->reg, 0x6); // Write enable
		spi_tinyvision_deselect(cfg->reg, 0);
		/* let deselect be accounted for */
		time = DELAY_SETTLE_SHORT;
		while (time > 0) {
			time--;
		}
		spi_tinyvision_select(cfg->reg, 0);
		spi_tinyvision_write(cfg->reg, 0x2); // program page
		size_t point = offset + i;
		spi_tinyvision_write(cfg->reg, (point & 0xFF0000) >> 16);
		spi_tinyvision_write(cfg->reg, (point & 0xFF00)  >> 8);
		spi_tinyvision_write(cfg->reg, (point & 0xFF));
		for (size_t j = 0; j < WRITE_SIZE && i + j < len; j++) {
			spi_tinyvision_write(cfg->reg, ((uint8_t*)data)[i+j]);
		}
		spi_tinyvision_deselect(cfg->reg, 0);
		/* let deselect be accounted for */
		time = DELAY_SETTLE_SHORT;
		while (time > 0) {
			time--;
		}
	}

	int status = 1;
	while(status & 1) {
		spi_tinyvision_select(cfg->reg, 0);
		spi_tinyvision_write(cfg->reg, 5);
		status = spi_tinyvision_read(cfg->reg);
		spi_tinyvision_deselect(cfg->reg, 0);
	}

	time = DELAY_SETTLE;
	while (time > 0) {
		time--;
	}

	irq_unlock(lock_key);

	return ret;
}

static int flash_tinyvision_erase(const struct device *dev, off_t offset, size_t size)
{
	const struct spi_tinyvision_config *cfg = dev->config;
	int ret = 0;

	if (size == 0U) {
		return 0;
	}

	if (!flash_tinyvision_valid_range(offset, size)) {
		return -EINVAL;
	}

	if (size % ERASE_SIZE) {
		return -EINVAL;
	}

	uint32_t lock_key = irq_lock();

	/* clear icache */
	__asm__ volatile(
		".insn 0x100F\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
	);

	volatile int time = DELAY_SETTLE;
	while (time > 0) {
		time--;
	}

	for (size_t i = 0; i < size; i += ERASE_SIZE) {
		spi_tinyvision_select(cfg->reg, 0);
		spi_tinyvision_write(cfg->reg, 0x6); // Write enable
		spi_tinyvision_deselect(cfg->reg, 0);
		/* let deselect be accounted for */
		time = DELAY_SETTLE_SHORT;
		while (time > 0) {
			time--;
		}
		spi_tinyvision_select(cfg->reg, 0);
		spi_tinyvision_write(cfg->reg, 0x20); // erase 4k sector
		size_t point = offset + i;
		spi_tinyvision_write(cfg->reg, (point & 0xFF0000) >> 16);
		spi_tinyvision_write(cfg->reg, (point & 0xFF00)  >> 8);
		spi_tinyvision_write(cfg->reg, (point & 0xFF));
		spi_tinyvision_deselect(cfg->reg, 0);
		/* let deselect be accounted for */
		time = DELAY_SETTLE_SHORT;
		while (time > 0) {
			time--;
		}
	}

	int status = 1;
	while(status & 1) {
		spi_tinyvision_select(cfg->reg, 0);
		spi_tinyvision_write(cfg->reg, 5);
		status = spi_tinyvision_read(cfg->reg);
		spi_tinyvision_deselect(cfg->reg, 0);
	}

	time = DELAY_SETTLE;
	while (time > 0) {
		time--;
	}

	irq_unlock(lock_key);

	return ret;
}

#if CONFIG_FLASH_PAGE_LAYOUT
static struct flash_pages_layout flash_tinyvision_pages_layout = {
	.pages_count = TOTAL_SIZE / ERASE_SIZE,
	.pages_size = ERASE_SIZE,
};

void flash_tinyvision_page_layout(const struct device *dev,
			     const struct flash_pages_layout **layout,
			     size_t *layout_size)
{
	*layout = &flash_tinyvision_pages_layout;
	*layout_size = 1;
}
#endif /* CONFIG_FLASH_PAGE_LAYOUT */

static const struct flash_parameters*
flash_tinyvision_get_parameters(const struct device *dev)
{
	ARG_UNUSED(dev);

	return &flash_tinyvision_parameters;
}

static DEVICE_API(flash, flash_tinyvision_driver_api) = {
	.read = flash_tinyvision_read,
	.write = flash_tinyvision_write,
	.erase = flash_tinyvision_erase,
	.get_parameters = flash_tinyvision_get_parameters,
#ifdef CONFIG_FLASH_PAGE_LAYOUT
	.page_layout = flash_tinyvision_page_layout,
#endif
};

static int flash_tinyvision_init(const struct device *dev)
{
	//struct flash_tinyvision_data *data = dev->data;
	//const struct spi_tinyvision_config *cfg = dev->config;

	//write_u32(9, cfg->reg + SPI_TINYVISION_CLK_DIVIDER);

	return 0;
}

DEVICE_DT_INST_DEFINE(0, flash_tinyvision_init, NULL,
		      NULL, &flash_config, POST_KERNEL,
		      CONFIG_FLASH_INIT_PRIORITY, &flash_tinyvision_driver_api);
