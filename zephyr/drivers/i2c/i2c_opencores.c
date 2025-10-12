/*
 * Copyright (c) 2021, Nordic Semiconductor ASA
 * Copyright (c) 2024 tinyVision.ai Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>
#include <zephyr/irq.h>

#include <soc.h>

LOG_MODULE_REGISTER(i2c_opencores, CONFIG_I2C_LOG_LEVEL);

/*
 * Definitions for the Opencores i2c master core
 */

/* --- Definitions for i2c master's registers --- */

/* ----- Read-write access */

#define VERSION_MUX(litex_reg, opencores_reg) \
((GET_I2C_CFG(dev)->rtl_version == 0) ? (litex_reg) : (opencores_reg))

#define OC_I2C_PRER_LO VERSION_MUX(0x00, 0x00) /* Low byte clock prescaler register */
#define OC_I2C_PRER_HI VERSION_MUX(0x01, 0x04) /* High byte clock prescaler register */
#define OC_I2C_CTR     VERSION_MUX(0x04, 0x08) /* Control register */

/* ----- Write-only registers */

#define OC_I2C_TXR   VERSION_MUX(0x08, 0x0C) /* Transmit byte register */
#define OC_I2C_CR    VERSION_MUX(0x10, 0x10) /* Command register */
#define OC_I2C_RESET 0x18 /* Reset register					 */

/* ----- Read-only registers */

#define OC_I2C_RXR VERSION_MUX(0x0c, 0x0C) /* Receive byte register */
#define OC_I2C_SR VERSION_MUX(0x14, 0x14) /* Status register */

/* ----- Bits definition */

/* ----- Control register */

#define OC_I2C_EN  (1 << 7) /* Core enable bit: */
			    /*      1 - core is enabled */
			    /*      0 - core is disabled */
#define OC_I2C_IEN (1 << 6) /* Interrupt enable bit */
			    /*      1 - Interrupt enabled */
			    /*      0 - Interrupt disabled */
			    /* Other bits in CR are reserved */

/* ----- Command register bits */

#define OC_I2C_STA  (1 << 7) /* Generate (repeated) start condition*/
#define OC_I2C_STO  (1 << 6) /* Generate stop condition */
#define OC_I2C_RD   (1 << 5) /* Read from slave */
#define OC_I2C_WR   (1 << 4) /* Write to slave */
#define OC_I2C_ACK  (1 << 3) /* Acknowledge from slave */
			     /*      1 - ACK */
			     /*      0 - NACK */
#define OC_I2C_IACK (1 << 0) /* Interrupt acknowledge */

/* ----- Status register bits */

#define OC_I2C_RXACK  (1 << 7) /* ACK received from slave */
			       /*      1 - ACK */
			       /*      0 - NACK */
#define OC_I2C_BUSY   (1 << 6) /* Busy bit */
#define OC_I2C_TIP    (1 << 1) /* Transfer in progress */
#define OC_I2C_IF     (1 << 0) /* Interrupt flag */
#define DT_DRV_COMPAT opencores_i2c

#include "i2c-priv.h"

struct i2c_opencores {
	uint32_t dev_config;
	uint32_t t_buf_delay;
	uint8_t next_cr;
};

struct i2c_opencores_cfg {
	volatile uint32_t *base;
	uint32_t bitrate;
	uint8_t rtl_version;
};

#define GET_I2C_CFG(dev) ((const struct i2c_opencores_cfg *)dev->config)

#define GET_I2C_OPENCORES(dev) ((struct i2c_opencores *)dev->data)

static void opencores_write(const struct device *dev, uint8_t reg, uint8_t value)
{
	const struct i2c_opencores_cfg *cfg = GET_I2C_CFG(dev);
	volatile uint8_t *register_map = (uint8_t *)cfg->base;
	register_map[reg] = value;
}

static uint8_t opencores_read(const struct device *dev, uint8_t reg)
{
	const struct i2c_opencores_cfg *cfg = GET_I2C_CFG(dev);
	volatile uint8_t *register_map = (uint8_t *)cfg->base;
	return register_map[reg];
}

static void opencores_16b_write(const struct device *dev, uint8_t reg, uint16_t value)
{
	const struct i2c_opencores_cfg *cfg = GET_I2C_CFG(dev);
	volatile uint16_t *register_map = (uint16_t *)cfg->base;
	register_map[reg] = value;
}

#define NS_TO_SYS_CLOCK_HW_CYCLES(ns)                                                              \
	((uint64_t)sys_clock_hw_cycles_per_sec() * (ns) / NSEC_PER_SEC + 1)

static void i2c_delay(unsigned int cycles_to_wait)
{
	uint32_t start = k_cycle_get_32();

	/* Wait until the given number of cycles have passed */
	while (k_cycle_get_32() - start < cycles_to_wait) {
	}
}

static int opencores_i2c_configure(const struct device *dev, uint32_t dev_config)
{
	struct i2c_opencores *context = GET_I2C_OPENCORES(dev);
	/* Check for features we don't support */
	if (I2C_ADDR_10_BITS & dev_config) {
		return -ENOTSUP;
	}

	uint16_t clock_cycles = 0;
	/* Setup speed to use */
	switch (I2C_SPEED_GET(dev_config)) {
	case I2C_SPEED_STANDARD:
		clock_cycles = NS_TO_SYS_CLOCK_HW_CYCLES(10000);
		context->t_buf_delay = NS_TO_SYS_CLOCK_HW_CYCLES(4700);
		break;
	case I2C_SPEED_FAST:
		clock_cycles = NS_TO_SYS_CLOCK_HW_CYCLES(2500);
		context->t_buf_delay = NS_TO_SYS_CLOCK_HW_CYCLES(1300);
		break;
	case I2C_SPEED_FAST_PLUS:
		clock_cycles = NS_TO_SYS_CLOCK_HW_CYCLES(1000);
		context->t_buf_delay = NS_TO_SYS_CLOCK_HW_CYCLES(500);
		break;
	case I2C_SPEED_HIGH:
		clock_cycles = NS_TO_SYS_CLOCK_HW_CYCLES(294.117);
		context->t_buf_delay = NS_TO_SYS_CLOCK_HW_CYCLES(500);
		break;
	case I2C_SPEED_ULTRA:
		clock_cycles = NS_TO_SYS_CLOCK_HW_CYCLES(200);
		context->t_buf_delay = NS_TO_SYS_CLOCK_HW_CYCLES(500);
		break;
	default:
		return -ENOTSUP;
	}
	opencores_write(dev, OC_I2C_RESET, 1);
	opencores_write(dev, OC_I2C_RESET, 0);
	opencores_write(dev, OC_I2C_CTR, 0);

	uint16_t prescale = ((clock_cycles + 4) / 5) - 1;
	if (GET_I2C_CFG(dev)->rtl_version == 0) {
		opencores_16b_write(dev, OC_I2C_PRER_LO, prescale);
	} else {
		opencores_write(dev, OC_I2C_PRER_LO, prescale & 0xff);
		opencores_write(dev, OC_I2C_PRER_HI, prescale >> 8);
	}
	context->dev_config = dev_config;

	opencores_write(dev, OC_I2C_CTR, OC_I2C_EN);
	return 0;
}

static bool i2c_wait_busy(const struct device *dev)
{
	int waits = 0;
	struct i2c_opencores *context = GET_I2C_OPENCORES(dev);
	while (waits < 10000 &&
	       (opencores_read(dev, OC_I2C_SR) & (OC_I2C_TIP | OC_I2C_BUSY)) != 0) {
		k_busy_wait(10);
		waits++;
	}
	if (waits >= 10000) {
		LOG_ERR("Timeout waiting for opencores i2c busy flag");
		return false;
	}
	// k_busy_wait(10);
	i2c_delay(context->t_buf_delay);
	LOG_DBG("Wait %d cycles for busy", waits);
	return true;
}

static bool i2c_wait_tip(const struct device *dev)
{
	int waits = 0;
	while (waits < 10000 && (opencores_read(dev, OC_I2C_SR) & (OC_I2C_TIP)) != 0) {
		k_busy_wait(10);
		waits++;
	}
	if (waits >= 10000) {
		LOG_ERR("Timeout waiting for opencores i2c TIP flag");
		return false;
	}
	// LOG_DBG("Wait %d cycles", waits);
	return true;
}

static int opencores_i2c_get_config(const struct device *dev, uint32_t *config)
{
	struct i2c_opencores *context = GET_I2C_OPENCORES(dev);
	*config = context->dev_config;
	return 0;
}

static void i2c_write_ack(const struct device *dev, bool nack)
{
	struct i2c_opencores *context = GET_I2C_OPENCORES(dev);
	context->next_cr |= nack ? OC_I2C_ACK : 0;
}

static bool i2c_write_byte(const struct device *dev, uint8_t byte)
{
	struct i2c_opencores *context = GET_I2C_OPENCORES(dev);
	opencores_write(dev, OC_I2C_TXR, byte);
	uint8_t cr_flags = OC_I2C_WR | context->next_cr;

	opencores_write(dev, OC_I2C_CR, cr_flags);
	context->next_cr = 0;
	if (!i2c_wait_tip(dev)) {
		return false;
	}

	bool has_ack = (opencores_read(dev, OC_I2C_SR) & OC_I2C_RXACK) != OC_I2C_RXACK;
	// LOG_DBG("Write byte 0x%x with flags %x (ack %d)", byte, cr_flags, has_ack);
	return has_ack;
}

static uint8_t i2c_read_byte(const struct device *dev)
{
	struct i2c_opencores *context = GET_I2C_OPENCORES(dev);
	uint8_t cr_flags = OC_I2C_RD | context->next_cr;
	opencores_write(dev, OC_I2C_CR, cr_flags);
	context->next_cr = 0;
	if (!i2c_wait_tip(dev)) {
		return false;
	}
	uint8_t rtn = opencores_read(dev, OC_I2C_RXR);
	// LOG_DBG("Read byte 0x%x with flags %x", rtn, cr_flags);
	return rtn;
}

static void i2c_start(const struct device *dev)
{
	struct i2c_opencores *context = GET_I2C_OPENCORES(dev);
	context->next_cr |= OC_I2C_STA;
}

static void i2c_repeated_start(const struct device *dev)
{
	struct i2c_opencores *context = GET_I2C_OPENCORES(dev);
	context->next_cr |= OC_I2C_STA;
}

static bool i2c_stop(const struct device *dev)
{
	// LOG_DBG("Generate stop");
	opencores_write(dev, OC_I2C_CR, OC_I2C_STO);
	return i2c_wait_busy(dev);
}

static int opencores_i2c_transfer(const struct device *dev, struct i2c_msg *msgs, uint8_t num_msgs,
				  uint16_t slave_address)
{
	uint8_t *buf, *buf_end;
	unsigned int flags;
	if (!num_msgs) {
		return 0;
	}
	int result = -EIO;
	/* We want an initial Start condition */
	flags = I2C_MSG_RESTART | I2C_MSG_STOP;

	do {
		/* Stop flag from previous message? */
		if (flags & I2C_MSG_STOP) {
			i2c_stop(dev);
		}

		/* Forget old flags except start flag */
		flags &= I2C_MSG_RESTART;

		/* Start condition? */
		if (flags & I2C_MSG_RESTART) {
			i2c_start(dev);
		} else if (msgs->flags & I2C_MSG_RESTART) {
			i2c_repeated_start(dev);
		}
		// LOG_DBG("Msg with flags %x for %x, length %d", flags, slave_address, msgs->len);
		/* Get flags for new message */
		flags |= msgs->flags;

		/* Send address after any Start condition */
		if (flags & I2C_MSG_RESTART) {
			unsigned int byte0 = slave_address << 1;

			byte0 |= (flags & I2C_MSG_RW_MASK) == I2C_MSG_READ;
			if (!i2c_write_byte(dev, byte0)) {
				goto finish; /* No ACK received */
			}
			flags &= ~I2C_MSG_RESTART;
		}

		/* Transfer data */
		buf = msgs->buf;
		buf_end = buf + msgs->len;
		if ((flags & I2C_MSG_RW_MASK) == I2C_MSG_READ) {
			/* Read */
			while (buf < buf_end) {
				i2c_write_ack(dev, (buf + 1) == buf_end);
				*buf++ = i2c_read_byte(dev);
			}
		} else {
			/* Write */
			while (buf < buf_end) {
				if (!i2c_write_byte(dev, *buf++)) {
					goto finish; /* No ACK received */
				}
			}
		}

		/* Next message */
		msgs++;
		num_msgs--;
	} while (num_msgs);

	result = 0;
finish:
	i2c_stop(dev);

	return result;
}

static DEVICE_API(i2c, opencores_i2c_api) = {
	.configure = opencores_i2c_configure,
	.get_config = opencores_i2c_get_config,
	.transfer = opencores_i2c_transfer,
};

static int i2c_opencores_init(const struct device *dev)
{
	const struct i2c_opencores_cfg *config = GET_I2C_CFG(dev);
	int ret = 0;
	LOG_DBG("bitrate = %d\nreg = %p\n", config->bitrate, (void *)config->base);
	ret = opencores_i2c_configure(dev,
				      I2C_MODE_CONTROLLER | i2c_map_dt_bitrate(config->bitrate));
	if (ret != 0) {
		LOG_ERR("failed to configure I2C opencores: %d", ret);
	}

	return ret;
}

#define OPENCORES_I2C_INIT(n)                                                                      \
	static const struct i2c_opencores_cfg i2c_opencores_cfg_##n = {                            \
		.base = (volatile uint32_t *)DT_INST_REG_ADDR(n),                                  \
		.bitrate = DT_INST_PROP(n, clock_frequency),                                       \
		.rtl_version = DT_INST_PROP_OR(n, rtl_version, 0),                                       \
	};                                                                                         \
                                                                                                   \
	struct i2c_opencores i2c_opencores_##n;                                                    \
	I2C_DEVICE_DT_INST_DEFINE(n, i2c_opencores_init, NULL, &i2c_opencores_##n,                 \
				  &i2c_opencores_cfg_##n, PRE_KERNEL_1, CONFIG_I2C_INIT_PRIORITY,  \
				  &opencores_i2c_api);

DT_INST_FOREACH_STATUS_OKAY(OPENCORES_I2C_INIT)
